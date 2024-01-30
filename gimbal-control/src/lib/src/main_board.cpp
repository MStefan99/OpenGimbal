#include "lib/inc/main_board.hpp"


void main_board::init() {
    uart::setCallback([](const uart::DefaultCallback::buffer_type& buffer) -> void {
        // Only one command type for now
//        switch (static_cast<IncomingCommand>(buffer.buffer[0] & 0xf)) { // Switch command type
//            case (IncomingCommand::GetVariable): {
                switch (static_cast<VID>(buffer.buffer[1])) { // Switch variable
                    case (VID::Error): {
                        
                    }
                }
//            }
//        }
    });
}

void main_board::sleep() {
    const uint8_t buf[] = {static_cast<uint8_t>(OutgoingCommand::Sleep) | (0x1 << 4u)};
    uart::send(buf, 1);
}


void main_board::calibrate() {
    const uint8_t buf[] = {static_cast<uint8_t>(OutgoingCommand::Calibrate) | (0x1 << 4u)};
    uart::send(buf, 1);
}
