import fs from 'fs';
import YAML from 'yaml';
import yargs from 'yargs';
import {hideBin} from 'yargs/helpers'

function generateDefinitions(yamlFile) {
	const content = fs.readFileSync(yamlFile, 'utf8');
	const data = YAML.parse(content);

	if (typeof data?.device !== 'object') {
		throw new Error(`Supplied file contains no device data`);
	}
	if (!data.device.name?.length) {
		throw new Error('Invalid device name');
	}
	if (Number.isInteger(data?.device.address)) {
		data.device.addresses = [data.device.address];
	} else if (!Array.isArray(data?.device?.addresses) || data?.device?.addresses?.some(a => !Number.isInteger(a))) {
		throw new Error(`Device ${data.device.name} has no or invalid addresses`);
	}

	const MAX_LENGTH = 70;
	const addressMap = new Map();

	let output = '';
	output += `/* File generated from device description on ${new Date().toLocaleString('en-GB')}`;
	data.device.version && (output += `, version ${data.device.version}`);
	output += ' */\n\n';
	output += `/* ${data.device.name}`;
	data.device.description && (output += `: ${data.device.description}`);
	output += ' */\n\n';

	const includeGuard = `${data.device.name}_REGS`;
	output += `#ifndef ${includeGuard}\n`;
	output += `#define ${includeGuard}\n\n`;

	if (data.device.addresses.length === 1) {
		output += `#define ${data.device.name}_ADDR`.padEnd(MAX_LENGTH, ' ') + `(0x${data.device.address.toString(16)})  /* ${data.device.name} address */\n\n`;
	} else {
		for (let i = 0; i < data.device.addresses.length; i++) {
			output += `#define ${data.device.name}_ADDR_${i}`.padEnd(MAX_LENGTH, ' ') + `(0x${data.device.addresses[i].toString(16)})  /* ${data.device.name} address */\n`;
		}
		output += '\n';
	}

	for (const [registerName, registerInfo] of Object.entries(data.device.registers)) {
		if (Number.isInteger(registerInfo?.address)) {
			registerInfo.addresses = [registerInfo.address];
		} else if (!Array.isArray(registerInfo?.addresses) || registerInfo.addresses.some(a => !Number.isInteger(a))) {
			throw new Error(`Register ${registerName} has no or invalid addresses`);
		}
		if (addressMap.has(registerInfo.address)) {
			throw new Error(`Register ${registerName} has the same address as ${addressMap.get(registerInfo.address).name}`);
		}
		for (const address of registerInfo.addresses) {
			addressMap.set(address, {name: registerName, info: registerInfo});
		}

		const regStr = `${data.device.name}_${registerName}`
		let regMask = 0;

		output += `/* ${data.device.name}_${registerName}`;
		registerInfo.mode && (output += `, ${registerInfo.mode.toUpperCase().split('').join('/')}`);
		registerInfo.description && (output += `: ${registerInfo.description}`);
		output += ' */\n';

		if (registerInfo.addresses.length === 1) {
			output += `#define ${data.device.name}_${registerName}_ADDR`.padEnd(MAX_LENGTH, ' ') + `(0x${registerInfo.address.toString(16)})`
			registerInfo.description && (output += `  /* (${regStr}) ${registerInfo.description} address */`);
			output += '\n';
		} else {
			for (let i = 0; i < registerInfo.addresses.length; i++) {
				output += `#define ${data.device.name}_${registerName}_ADDR_${i}`.padEnd(MAX_LENGTH, ' ') + `(0x${registerInfo.addresses[i].toString(16)})`
				registerInfo.description && (output += `  /* (${regStr}) ${registerInfo.description} address */`);
				output += '\n';
			}
		}
		if (!!registerInfo.reset) {
			output += `#define ${data.device.name}_${registerName}_RESETVALUE`.padEnd(MAX_LENGTH, ' ') + `(0x${registerInfo.reset.toString(16)})`;
			registerInfo.description && (output += `  /* (${regStr}) ${registerInfo.description} reset value */`);
			output += '\n';
		}
		output += '\n';

		if (!registerInfo?.groups || registerInfo.groups.length === 0) {
			console.warn(`Warning: Register ${registerName} has no bit groups`);
			regMask = (1 << (registerInfo.addresses.length * 8)) - 1;
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
				let optionMap = new Map();

				for (const option of groupInfo.options.sort((option1, option2) => option1?.value - option2?.value)) {
					if (!option?.name?.length) {
						throw new Error(`Not all options for group ${groupName} in ${registerName} have names`);
					}
					if (!Number.isInteger(option?.value)) {
						throw new Error(`Option ${option.name} for group ${groupName} in ${registerName} has no or invalid value`);
					}
					if (optionMap.has(option.value)) {
						throw new Error(`Option ${option.name} for group ${groupName} in ${registerName} has the same value as ${optionMap.get(option.value).name}`);
					}
					if (!option.description) {
						option.description = option.name;
					}
					optionMap.set(option.value, option);

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

	output += `#endif  // ${includeGuard}\n`;

	console.log('\nDefinitions generated successfully!');
	fs.writeFileSync(`${data.device.name}_regs.h`, output);
}

(() => {
	const args = yargs(hideBin(process.argv))
		.usage('Usage: $0 <file>.yaml')
		.demandOption(['y'])
		.describe('y', 'YAML file with device description')
		.nargs('y', 1)
		.alias('y', 'yaml')
		.parse();

	if (!args.y?.length) {
		return;
	}

	generateDefinitions(args.y);
})();
