from saleae.analyzers import HighLevelAnalyzer, AnalyzerFrame
from saleae.data.timing import SaleaeTimeDelta

variables = {
    0x0: {
        'name': 'Calibration'
    },
    0x2: {
        'name': 'Offset'
    },
    0x3: {
        'name': 'Position'
    },
    0x4: {
        'name': 'Speed'
    },
}


class MotorCommand:
    def __init__(self, bytes):
        self.len = ((bytes[0] & 0xf0) >> 4) + 1
        self.dest_addr = bytes[0] & 0x0f

        if len(bytes) > 1:
            self.src_addr = ((bytes[1] & 0xf0) >> 4) + 1
            self.cmd = bytes[1] & 0x0f


class IdleCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)


class SleepCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)


class MoveCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)

        if (len(bytes) == 4):
            self.torque = (bytes[2] & 0xf0) >> 4
            self.position = ((bytes[2] & 0x0f) << 8) | bytes[3]

    def __str__(self):
        return f'Torque: {self.torque}, Position: {self.position}'


class HapticCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)

        if (len(bytes) == 4):
            self.intensity = (bytes[2] & 0xf0) >> 4
            self.duration = ((bytes[2] & 0x0f) << 8) | bytes[3]

    def __str__(self):
        return f'Intensity: {self.intensity}, Duration: {self.duration}'


class ToneCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)

        if (len(bytes) == 4):
            self.frequency = (bytes[2] << 8) | bytes[3]

    def __str__(self):
        return f'Frequency: {self.frequency}'


class AdjustOffsetCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)

        if (len(bytes) == 4):
            self.offset = ((bytes[2] & 0x0f) << 8) | bytes[3]

    def __str__(self):
        return f'Offset: {self.offset}'


class CalibrateCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)

        if (len(bytes) == 3):
            self.mode = bytes[2] & 0x3

    def __str__(self):
        return f'Mode: {self.mode}'


class GetVariableCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)

        if (len(bytes) >= 3):
            self.variable = bytes[2] & 0x3

    def __str__(self):
        return f'Variable: {variables[self.variable]["name"]}'


class SetVariableCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)

        if (len(bytes) >= 3):
            self.variable = bytes[2] & 0x3
            self.value = int.from_bytes(bytes[3:], 'big')

    def __str__(self):
        return f'Variable: {variables[self.variable]["name"]}, Value: {self.value}'


class ReturnVariableCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)

        if (len(bytes) >= 3):
            self.variable = bytes[2] & 0x3
            self.value = int.from_bytes(bytes[3:], 'big')

    def __str__(self):
        return f'Variable: {variables[self.variable]["name"]}, Value: {self.value}'


commands = {
    0x0: {
        'name': 'Idle',
        'parse': lambda bytes: IdleCommand(bytes)
    },
    0x1: {
        'name': 'Sleep',
        'parse': lambda bytes: SleepCommand(bytes)
    },
    0x2: {
        'name': 'Move',
        'parse': lambda bytes: MoveCommand(bytes)
    },
    0x3: {
        'name': 'Haptic',
        'parse': lambda bytes: HapticCommand(bytes)
    },
    0x4: {
        'name': 'Tone',
        'parse': lambda bytes: ToneCommand(bytes)
    },
    0x5: {
        'name': 'Adjust offset',
        'parse': lambda bytes: AdjustOffsetCommand(bytes)
    },
    0x6: {
        'name': 'Calibrate',
        'parse': lambda bytes: CalibrateCommand(bytes)
    },
    0xe: {
        'name': 'Get variable',
        'parse': lambda bytes: GetVariableCommand(bytes)
    },
    0xf: {
        'name': 'Set variable',
        'parse': lambda bytes: SetVariableCommand(bytes)
    }
}

responses = {
    0xf: {
        'name': 'Return variable',
        'parse': lambda bytes: ReturnVariableCommand(bytes)
    }
}


class Hla(HighLevelAnalyzer):
    result_types = {
        'cmd': {
            'format': 'Length: {{data.len}}, To: {{data.dest_addr}}'
        },
        'src': {
            'format': 'From: {{data.src_addr}}, Command: {{data.cmd}}'
        },
        'data': {
            'format': '{{data.str}}'
        }
    }

    def __init__(self, **kwargs):
        self.start_time = 0
        self.data_start = 0
        self.bytes = []
        self.remaining_bytes = 0
        self.is_response = False

    def decode(self, frame: AnalyzerFrame):
        if not frame.type == 'data':
            return  # Not interested in anything but data

        byte = int.from_bytes(frame.data['data'], 'big')

        if not len(self.bytes) or frame.start_time - self.start_time > SaleaeTimeDelta(0.001):  # First byte
            self.remaining_bytes = ((byte & 0xf0) >> 4) + 1
            self.is_response = (byte & 0xf) == 0
            self.start_time = frame.start_time

        self.bytes.append(byte)
        self.remaining_bytes -= 1

        if (len(self.bytes) == 1):
            command = MotorCommand(self.bytes)
            return AnalyzerFrame('cmd', frame.start_time, frame.end_time, {
                'len': command.len,
                'dest_addr': command.dest_addr,
            })

        if (len(self.bytes) == 2):
            command = MotorCommand(self.bytes)
            message = responses[self.bytes[1] & 0xf] if self.is_response else commands[self.bytes[1] & 0xf]
            return AnalyzerFrame('src', frame.start_time, frame.end_time, {
                'src_addr': command.src_addr,
                'cmd': message['name']
            })

        if (len(self.bytes) == 3):
            self.data_start = frame.start_time

        if not self.remaining_bytes:
            # Parse bytes
            generic_command = MotorCommand(self.bytes)
            specific_command = responses[generic_command.cmd]['parse'](self.bytes) if self.is_response else \
                commands[generic_command.cmd]['parse'](self.bytes)
            self.bytes = []
            return AnalyzerFrame('data', self.data_start, frame.end_time, {
                'str': str(specific_command)
            })
