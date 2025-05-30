from saleae.analyzers import HighLevelAnalyzer, AnalyzerFrame
from saleae.data.timing import SaleaeTimeDelta

variables = {
    0x0: {
        'name': 'Calibration'
    },
    0x1: {
        'name': 'Offset'
    },
    0x2: {
        'name': 'Position'
    },
    0x3: {
        'name': 'Speed'
    },
}


class MotorCommand:
    def __init__(self, bytes):
        self.len = ((bytes[0] & 0xf0) >> 4) + 1
        self.dest_addr = bytes[0] & 0x0f

        if len(bytes) > 1:
            self.src_addr = ((bytes[1] & 0xf0) >> 4)
            self.cmd = bytes[1] & 0x0f


class SleepCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)


class IdleCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)


class WakeCommand(MotorCommand):
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


class ToneCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)

        if (len(bytes) == 4):
            self.frequency = (bytes[2] << 8) | bytes[3]

    def __str__(self):
        return f'Frequency: {self.frequency}'


class HapticCommand(MotorCommand):
    def __init__(self, bytes):
        super().__init__(bytes)

        if (len(bytes) == 4):
            self.intensity = (bytes[2] & 0xf0) >> 4
            self.duration = ((bytes[2] & 0x0f) << 8) | bytes[3]

    def __str__(self):
        return f'Intensity: {self.intensity}, Duration: {self.duration}'


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
        'name': 'Sleep',
        'parse': lambda bytes: SleepCommand(bytes)
    },
    0x1: {
        'name': 'Idle',
        'parse': lambda bytes: IdleCommand(bytes)
    },
    0x2: {
        'name': 'Wake',
        'parse': lambda bytes: WakeCommand(bytes)
    },
    0x3: {
        'name': 'Move',
        'parse': lambda bytes: MoveCommand(bytes)
    },
    0x4: {
        'name': 'Tone',
        'parse': lambda bytes: ToneCommand(bytes)
    },
    0x5: {
        'name': 'Haptic',
        'parse': lambda bytes: HapticCommand(bytes)
    },
    0x6: {
        'name': 'Adjust offset',
        'parse': lambda bytes: AdjustOffsetCommand(bytes)
    },
    0x7: {
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

unknown_message = {
    'name': 'Unknown',
    'parse': lambda bytes: None
}


class Hla(HighLevelAnalyzer):
    result_types = {
        'cmd': {
            'format': 'Length: {{data.len}}, To: {{data.dest_addr}} '
        },
        'src': {
            'format': 'From: {{data.src_addr}}, Command: {{data.cmd}} '
        },
        'data': {
            'format': '{{data.str}} '
        }
    }

    def __init__(self):
        self.start_time = 0
        self.last_end_time = 0
        self.data_start = 0
        self.bytes = []
        self.remaining_bytes = 0

    def decode(self, frame: AnalyzerFrame):
        if not frame.type == 'data':
            return  # Skip non-data frames

        if frame.data.get('error', None):
            return  # Skip frames with errors

        byte = int.from_bytes(frame.data['data'], 'big')

        if not self.remaining_bytes or (frame.start_time - self.last_end_time) > SaleaeTimeDelta(millisecond=5):  # First byte
            self.remaining_bytes = ((byte & 0xf0) >> 4) + 1
            self.start_time = frame.start_time
            self.bytes = []

        self.bytes.append(byte)
        self.remaining_bytes -= 1
        self.last_end_time = frame.end_time

        if (len(self.bytes) == 1):
            command = MotorCommand(self.bytes)
            return AnalyzerFrame('cmd', frame.start_time, frame.end_time, {
                'len': command.len,
                'dest_addr': command.dest_addr,
            })

        if (len(self.bytes) == 2):
            command = MotorCommand(self.bytes)
            message = responses.get(command.cmd, unknown_message) if command.src_addr != 0 else commands.get(command.cmd, unknown_message)
            return AnalyzerFrame('src', frame.start_time, frame.end_time, {
                'src_addr': command.src_addr,
                'cmd': message['name']
            })

        if (len(self.bytes) == 3):
            self.data_start = frame.start_time

        if not self.remaining_bytes:
            command = MotorCommand(self.bytes)
            message = responses.get(command.cmd, unknown_message)['parse'](self.bytes) if command.src_addr != 0 else \
                commands.get(command.cmd, unknown_message)['parse'](self.bytes)
            self.bytes = []
            return AnalyzerFrame('data', self.data_start, frame.end_time, {
                'str': str(message) if message else 'Unknown'
            })
