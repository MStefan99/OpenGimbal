#include "motor.hpp"

static uint16_t sleepingMotors {0xfffe};

static void setBaud(uint8_t address) {
	if (sleepingMotors & (0x1 << address)) {
		uart::slow();
	} else {
		uart::fast();
	}
}

static void setSleeping(uint8_t address, bool sleeping) {
	uint16_t mask = address == 15 ? 0xfffe : (0x1 << address);

	if (sleeping) {
		sleepingMotors |= mask;
	} else {
		sleepingMotors &= ~mask;
	}
}

void motor::sleep(uint8_t address) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::Sleep, 0}
	};

	setBaud(address);
	uart::sendToMotors(command.getBuffer(), command.getLength());
	setSleeping(address, true);
}

void motor::move(uint8_t address, uint16_t position, uint8_t torque) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::Move, 2}
	};
	auto& buf {command.getBuffer()};

	buf[2] = (torque << 4u) | (position >> 8u);
	buf[3] = position;

	setBaud(address);
	uart::sendToMotors(command.getBuffer(), command.getLength());
	setSleeping(address, false);
}

void motor::tone(uint8_t address, uint16_t frequency) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::Tone, 2}
	};
	auto& buf {command.getBuffer()};

	buf[2] = frequency >> 8u;
	buf[3] = frequency;

	setBaud(address);
	uart::sendToMotors(command.getBuffer(), command.getLength());
}

void motor::haptic(uint8_t address, uint8_t intensity, uint16_t duration) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::Haptic, 2}
	};
	auto& buf {command.getBuffer()};

	buf[2] = (intensity << 4u) | (duration >> 8u);
	buf[3] = duration;

	setBaud(address);
	uart::sendToMotors(command.getBuffer(), command.getLength());
	setSleeping(address, false);
}

void motor::adjustOffset(uint8_t address, uint16_t targetPosition) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::AdjustOffset, 2}
	};
	auto& buf {command.getBuffer()};

	buf[2] = targetPosition >> 8u;
	buf[3] = targetPosition;

	setBaud(address);
	uart::sendToMotors(command.getBuffer(), command.getLength());
}

void motor::calibrate(uint8_t address, uint8_t mode) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::Calibrate, 1}
	};
	auto& buf {command.getBuffer()};

	buf[2] = mode;

	setBaud(address);
	uart::sendToMotors(command.getBuffer(), command.getLength());
	setSleeping(address, false);
}

void motor::send(const uint8_t* buf, uint8_t len) {
	uint8_t address = buf[0] & 0xf;

	if (!address) {  // Check destination address
		return;
	}

	uart::sendToMotors(buf, len);

	auto command = static_cast<MotorCommand::CommandType>(buf[1] & 0xf);
	if (command == MotorCommand::CommandType::Move || command == MotorCommand::CommandType::Haptic
	    || command == MotorCommand::CommandType::Calibrate) {
		setSleeping(address, false);
	} else if (command == MotorCommand::CommandType::Sleep) {
		setSleeping(address, true);
	}

	setBaud(address);
}
