import {reactive, ref} from 'vue';
import {IGimbal} from './Gimbal';
import {IMotorManager} from './MotorManager';

export const activeMotorManager = reactive<IMotorManager[]>([]);
export const activeGimbal = ref<IGimbal | null>(null);
