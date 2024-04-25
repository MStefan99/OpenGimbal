import fs from 'fs';
import YAML from 'yaml';
import yargs from 'yargs';

function generateDefinitions(yamlFile) {
	const content = fs.readFileSync(yamlFile, 'utf8');
	const data = YAML.parse(content);

	if (typeof data.device !== 'object') {
		throw new Error(`Supplied file contains no device data`);
	}
	if (!Number.isInteger(data.device.address)) {
		throw new Error(`Device ${data.device.name} has no or invalid address`);
	}

	const MAX_LENGTH = 60;

	let output = '';
	output += `/* File generated from device description on ${new Date().toLocaleString('en-GB')}`;
	data.device.version && (output += `, version ${data.device.version}`);
	output += ' */\n\n';
	output += `/* ${data.device.name}`;
	data.device.description && (output += `: ${data.device.description}`);
	output += ' */\n\n';

	output += `#define ${data.device.name}_ADDR`.padEnd(MAX_LENGTH, ' ') + `(0x${data.device.address.toString(16)})  /* ${data.device.name} address */\n\n`;

	for (const [registerName, registerInfo] of Object.entries(data.device.registers)) {
		if (!Number.isInteger(registerInfo.address)) {
			throw new Error(`Register ${registerName} has no or invalid address`);
		}
		if (!Number.isInteger(registerInfo.mask)) {
			throw new Error(`Register ${registerName} has no or invalid mask value`);
		}

		const reg = `${data.device.name}_${registerName}`

		output += `/* ${data.device.name}_${registerName}`;
		registerInfo.mode && (output += `, ${registerInfo.mode.toUpperCase()}`);
		registerInfo.description && (output += `: ${registerInfo.description}`);
		output += ' */\n';

		output += `#define ${data.device.name}_${registerName}_ADDRESS`.padEnd(MAX_LENGTH, ' ') + `(0x${registerInfo.address.toString(16)})`
		registerInfo.description && (output += `  /* (${reg}) ${registerInfo.description} address */`);
		output += '\n';
		if (registerInfo.reset?.length) {
			output += `#define ${data.device.name}_${registerName}_RESETVALUE`.padEnd(MAX_LENGTH, ' ') + `(0x${registerInfo.reset.toString(16)})`;
			registerInfo.description && (output += `  /* (${reg}) ${registerInfo.description} reset value */`);
			output += '\n';
		}
		output += '\n';

		if (!registerInfo.groups || registerInfo.groups.length === 0) {
			throw new Error(`Register ${registerName} has no bit groups`);
		}

		for (const [groupName, groupInfo] of Object.entries(registerInfo.groups)) {
			if (!Number.isInteger(groupInfo.offset)) {
				throw new Error(`Group ${groupName} in ${registerName} has no or invalid offset`);
			}
			if (!Number.isInteger(groupInfo.size)) {
				throw new Error(`Group ${groupName} in ${registerName} has no or invalid size`);
			}

			const group = `${reg}_${groupName}`
			const pos = group + `_Pos`;
			const msk = group + `_Msk`;

			output += `#define ${pos}`.padEnd(MAX_LENGTH, ' ') + `(${groupInfo.offset})`
			groupInfo.description && (output += `  /* (${reg}) ${groupInfo.description} position */`);
			output += `\n`;
			output += `#define ${msk}`.padEnd(MAX_LENGTH, ' ') + `((0x${(Math.pow(2, groupInfo.size) - 1).toString(16)}) << ${pos})`;
			groupInfo.description && (output += `  /* (${reg}) ${groupInfo.description} mask */`);
			output += `\n`;
			output += `#define ${group}(value)`.padEnd(MAX_LENGTH, ' ') + `(${msk} & ((value) << ${pos}))`;
			groupInfo.description && (output += `  /* Assignment of ${groupName} value in ${reg} register */`);
			output += `\n`;

			if (groupInfo.options) {
				groupInfo.options.forEach(option => {
					if (!option.name?.length) {
						throw new Error(`Not all options for group ${groupName} in ${registerName} have names`);
					}
					if (!Number.isInteger(option.value)) {
						throw new Error(`Option ${option.name} for group ${groupName} in ${registerName} has no or invalid value`);
					}
					output += `#define   ${group}_${option.name}_Val`.padEnd(MAX_LENGTH, ' ') + `(${option.value})`
					option.description && (output += `  /* (${reg}) ${option.description} value */`);
					output += `\n`;
				});
				groupInfo.options.forEach(option => {
					output += `#define ${group}_${option.name}`.padEnd(MAX_LENGTH, ' ') + `(${group}_${option.name}_Val << ${pos})`;
					option.description && (output += `  /* (${reg}) ${option.description} */`);
					output += `\n`;
				});
			}
		}

		output += `#define ${reg}_Msk`.padEnd(MAX_LENGTH, ' ') + `(0x${registerInfo.mask.toString(16)})  /* (${reg}) Register mask */\n\n\n`;
	}

	console.log('Definitions generated successfully!');
	fs.writeFileSync('generated_definitions.h', output);
}

// Replace 'input.yaml' with the path to file from yargs
generateDefinitions('input.yaml');
