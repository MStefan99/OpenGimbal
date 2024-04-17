import StateVariable from './StateVariable';
import createState, {updateState} from './state';
import Server from './server';
import {traverse} from './util';
import {openPort} from '../core/Port';
import {MotorManager} from '../core/MotorManager';
import {CalibrationBits} from '../core/BitMask';
import {Motor} from '../core/Motor';

const server = new Server((variable, value) => {
	updateState(state, variable, value);
	server.broadcastUpdate(variable, traverse(state, variable));
});

type MotorState = {
	address: StateVariable<number>;
	sleeping: StateVariable<boolean>;
	enabled: StateVariable<boolean>;
	position: StateVariable<number>;
	torque: StateVariable<number>;
	frequency: StateVariable<number>;
	hapticIntensity: StateVariable<number>;
	hapticDuration: StateVariable<number>;
	calibration: StateVariable<CalibrationBits[]>;
	offset: StateVariable<number>;
	range: StateVariable<number>;
};

// @ts-ignore
const state = createState<{motors: StateVariable<MotorState[]>}>(
	{
		motors: new StateVariable<MotorState[]>()
	},
	() => server.broadcastState(state)
);

server.broadcastState(state);

async function defaultMotorState(motor: Motor): Promise<MotorState> {
	// @ts-ignore
	const state = {
		address: new StateVariable<number>(motor.address).writable(false),
		sleeping: new StateVariable<boolean>(false).effect(async sleeping => sleeping && await motor.sleep()),
		enabled: new StateVariable(true).effect(async enabled => !enabled && await motor.disable()),
		position: new StateVariable(0).effect(async position => await motor.move(position, state.torque.get())),
		torque: new StateVariable(0),
		hapticIntensity: new StateVariable(0).effect(async intensity => await motor.haptic(intensity, state.hapticDuration.get())),
		hapticDuration: new StateVariable(0),
		calibration: new StateVariable(await motor.getCalibration()),
		offset: new StateVariable(await motor.getOffset()),
		range: new StateVariable(await motor.getRange()),
	};
	return state;
}

openPort('COM3', (port) => {
	port.on('close', () => console.log(port.path, 'closed'));

	port.on('open', async () => {
		console.log(port.path, 'opened');

		const manager = new MotorManager(port);
		port.on('data', (data: Buffer) => manager.parse(data));

		await manager.enumerate();
		{
			const motorStates: MotorState[] = [];
			for (const motor of manager.active) {
				motorStates.push(await defaultMotorState(motor));
			}
			state.motors.set(motorStates);
		}

		if (!manager.active.length) {
			console.error('No motors found');
			port.close();
		}
	});

	port.on('error', function (err: Error) {
		console.log('Error: ', err.message);
	});
});
