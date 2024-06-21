#include "motor.hpp"


void motor::sleep(uint8_t address) {
    auto command {MotorCommand {0, address, MotorCommand::CommandType::Sleep, 0}};
    
    uart::sendToMotors(command.getBuffer(), command.getLength());
}

void motor::move(uint8_t address, uint16_t position, uint8_t torque) {
    auto command {MotorCommand {0, address, MotorCommand::CommandType::Move, 2}};
    auto& buf {command.getBuffer()};
    
    buf[2] = (torque << 4u) | (position >> 8u);
    buf[3] = position;
    
    uart::sendToMotors(command.getBuffer(), command.getLength());
}

void motor::tone(uint8_t address, uint16_t frequency) {
    auto command {MotorCommand {0, address, MotorCommand::CommandType::Tone, 2}};
    auto& buf {command.getBuffer()};
    
    buf[2] = frequency >> 8u;
    buf[3] = frequency;
    
    uart::sendToMotors(command.getBuffer(), command.getLength());
}

void motor::haptic(uint8_t address, uint8_t intensity, uint16_t duration) {
    auto command {MotorCommand {0, address, MotorCommand::CommandType::Haptic, 2}};
    auto& buf {command.getBuffer()};
    
    buf[2] = (intensity << 4u) | (duration >> 8u);
    buf[3] = duration;
    
    uart::sendToMotors(command.getBuffer(), command.getLength());
}

void motor::adjustOffset(uint8_t address, uint16_t targetPosition) {
    auto command {MotorCommand {0, address, MotorCommand::CommandType::AdjustOffset, 2}};
    auto& buf {command.getBuffer()};
    
    buf[2] = targetPosition >> 8u;
    buf[3] = targetPosition;
    
    uart::sendToMotors(command.getBuffer(), command.getLength());
}

void motor::calibrate(uint8_t address, uint8_t mode) {
    auto command {MotorCommand {0, address, MotorCommand::CommandType::Calibrate, 1}};
    auto& buf {command.getBuffer()};
    
    buf[2] = mode;
    
    uart::sendToMotors(command.getBuffer(), command.getLength());
}
