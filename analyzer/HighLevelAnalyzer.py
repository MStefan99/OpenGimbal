from saleae.analyzers import HighLevelAnalyzer, AnalyzerFrame
from saleae.data.timing import SaleaeTimeDelta

commands = {
    0x0: {
        'name': 'Idle'
    },
    0x1: {
        'name': 'Sleep'
    },
    0x2: {
        'name': 'Move'
    },
    0x3: {
        'name': 'Haptic'
    },
    0x4: {
        'name': 'Tone'
    },
    0x5: {
        'name': 'Adjust offset'
    },
    0x6: {
        'name': 'Calibrate'
    },
    0xe: {
        'name': 'Get variable'
    },
    0xf: {
        'name': 'Set variable'
    }
}

responses = {
    0xf: {
        'name': 'Return variable'
    }
}


class Hla(HighLevelAnalyzer):
    result_types = {
        'cmd': {
            'format': 'Length: {{data.len}}, To: {{data.dest_addr}}'
        },
        'src': {
            'format': 'From: {{data.src_addr}}, Command: {{data.cmd}}'
        }
    }

    def __init__(self, **kwargs):
        self.start_time = 0
        self.bytes = []
        self.remaining_bytes = 0
        self.is_response = False

    def decode(self, frame: AnalyzerFrame):
        if not frame.type == 'data':
            return  # Not interested in anything but data

        byte = int.from_bytes(frame.data['data'], 'little')

        if not len(self.bytes) or frame.start_time - self.start_time > SaleaeTimeDelta(0.001):  # First byte
            self.remaining_bytes = ((byte & 0xf0) >> 4) + 1
            self.is_response = (byte & 0xf) == 0
            self.start_time = frame.start_time

        self.bytes.append(byte)
        self.remaining_bytes -= 1

        if (len(self.bytes) == 1):
            return AnalyzerFrame('cmd', frame.start_time, frame.end_time, {
                'len': self.remaining_bytes + 1,
                'dest_addr': self.bytes[0] & 0xf,
            })

        if (len(self.bytes) == 2):
            message = responses[self.bytes[1] & 0xf] if self.is_response else commands[self.bytes[1] & 0xf]
            return AnalyzerFrame('src', frame.start_time, frame.end_time, {
                'src_addr': (self.bytes[1] & 0xf0) >> 4,
                'cmd': message['name']
            })

        if not self.remaining_bytes:
            # Parse bytes
            self.bytes = []

        return AnalyzerFrame('len', frame.start_time, frame.end_time, {
            'r': self.remaining_bytes,
        })
