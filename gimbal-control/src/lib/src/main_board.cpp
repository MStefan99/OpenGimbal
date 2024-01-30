#include "lib/inc/main_board.hpp"


void main_board::init() {
    uart::setCallback([](const uart::DefaultCallback::buffer_type& buffer) -> void {
        switch (static_cast<IncomingCommand>(buffer.buffer[1] & 0xf)) { // Switch command type
            case (IncomingCommand::GetVariable): {
                
            }
        }
    });
}
