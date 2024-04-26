import fs from 'fs';
import YAML from 'yaml';

function generateDefinitions(yamlFile) {
	const content = fs.readFileSync(yamlFile, 'utf8');
	const data = YAML.parse(content);

	if (typeof data?.device !== 'object') {
		throw new Error(`Supplied file contains no device data`);
	}
	if (!Number.isInteger(data?.device?.address)) {
		throw new Error(`Device ${data.device.name} has no or invalid address`);
	}

	const MAX_LENGTH = 70;

	let output = '';
	output += `/* File generated from device description on ${new Date().toLocaleString('en-GB')}`;
	data.device.version && (output += `, version ${data.device.version}`);
	output += ' */\n\n';
	output += `/* ${data.device.name}`;
	data.device.description && (output += `: ${data.device.description}`);
	output += ' */\n\n';

	output += `#define ${data.device.name}_ADDR`.padEnd(MAX_LENGTH, ' ') + `(0x${data.device.address.toString(16)})  /* ${data.device.name} address */\n\n`;

	for (const [registerName, registerInfo] of Object.entries(data.device.registers)) {
		if (!Number.isInteger(registerInfo?.address)) {
			throw new Error(`Register ${registerName} has no or invalid address`);
		}

		const regStr = `${data.device.name}_${registerName}`
		let regMask = 0;

		output += `/* ${data.device.name}_${registerName}`;
		registerInfo.mode && (output += `, ${registerInfo.mode.toUpperCase().split('').join('/')}`);
		registerInfo.description && (output += `: ${registerInfo.description}`);
		output += ' */\n';

		output += `#define ${data.device.name}_${registerName}_ADDRESS`.padEnd(MAX_LENGTH, ' ') + `(0x${registerInfo.address.toString(16)})`
		registerInfo.description && (output += `  /* (${regStr}) ${registerInfo.description} address */`);
		output += '\n';
		if (!!registerInfo.reset) {
			output += `#define ${data.device.name}_${registerName}_RESETVALUE`.padEnd(MAX_LENGTH, ' ') + `(0x${registerInfo.reset.toString(16)})`;
			registerInfo.description && (output += `  /* (${regStr}) ${registerInfo.description} reset value */`);
			output += '\n';
		}
		output += '\n';

		if (!registerInfo?.groups || registerInfo.groups.length === 0) {
			console.warn(`Warning: Register ${registerName} has no bit groups`);
			regMask = 0xff;
		}

		for (const [groupName, groupInfo] of Object.entries(registerInfo.groups ?? {})
			?.sort(([name1, info1], [name2, info2]) => info1.offset - info2.offset)) {
			if (!Number.isInteger(groupInfo?.offset)) {
				throw new Error(`Group ${groupName} in ${registerName} has no or invalid offset`);
			}
			if (!Number.isInteger(groupInfo?.size)) {
				groupInfo.size = 1;
			}

			const groupStr = `${regStr}_${groupName}`
			const posStr = groupStr + `_Pos`;
			const mskStr = groupStr + `_Msk`;

			regMask |= (Math.pow(2, groupInfo.size) - 1) << groupInfo.offset;

			output += `#define ${posStr}`.padEnd(MAX_LENGTH, ' ') + `(${groupInfo.offset})`
			groupInfo.description && (output += `  /* (${regStr}) ${groupInfo.description} position */`);
			output += `\n`;
			output += `#define ${mskStr}`.padEnd(MAX_LENGTH, ' ') + `((0x${(Math.pow(2, groupInfo.size) - 1).toString(16)}) << ${posStr})`;
			groupInfo.description && (output += `  /* (${regStr}) ${groupInfo.description} mask */`);
			output += `\n`;
			output += `#define ${groupStr}(value)`.padEnd(MAX_LENGTH, ' ') + `(${mskStr} & ((value) << ${posStr}))`;
			groupInfo.description && (output += `  /* Assignment of ${groupName} value in ${regStr} register */`);
			output += `\n`;
			output += `#define ${groupStr}_Get(reg)`.padEnd(MAX_LENGTH, ' ') + `(((reg) & ${mskStr}) >> ${posStr})`;
			groupInfo.description && (output += `  /* Assignment of ${groupName} value in ${regStr} register */`);
			output += `\n`;

			if (groupInfo.options) {
				let valuesStr = '';
				let optionsStr = '';

				for (const option of groupInfo.options.sort((option1, option2) => option1?.value - option2?.value)) {
					if (!option?.name?.length) {
						throw new Error(`Not all options for group ${groupName} in ${registerName} have names`);
					}
					if (!Number.isInteger(option?.value)) {
						throw new Error(`Option ${option.name} for group ${groupName} in ${registerName} has no or invalid value`);
					}

					valuesStr += `#define   ${groupStr}_${option.name}_Val`.padEnd(MAX_LENGTH, ' ') + `(${option.value})`
					option.description && (valuesStr += `  /* (${regStr}) ${option.description} value */`);
					valuesStr += `\n`;
					optionsStr += `#define ${groupStr}_${option.name}`.padEnd(MAX_LENGTH, ' ') + `(${groupStr}_${option.name}_Val << ${posStr})`;
					option.description && (optionsStr += `  /* (${regStr}) ${option.description} */`);
					optionsStr += `\n`;
				}

				output += valuesStr + optionsStr;
			}
		}

		output += `#define ${regStr}_Msk`.padEnd(MAX_LENGTH, ' ') + `(0x${regMask.toString(16)})  /* (${regStr}) Register mask */\n\n\n`;
	}

	console.log('Definitions generated successfully!');
	fs.writeFileSync('generated_definitions.h', output);
}

// Replace 'input.yaml' with the path to file from yargs
generateDefinitions('input.yaml');
