#include "motor.hpp"

static uint32_t                             lastRequestSent {0};
static RingBuffer<MotorCommand, uint8_t, 4> requestQueue;
static uart::DefaultCallback::callback_type callback;

static void processResponse(const uart::DefaultCallback::buffer_type& buffer) {
	if (requestQueue.size()) {  // Should always be true
		requestQueue.pop_front();
		if (requestQueue.size()) {
			auto command {requestQueue.front()};
			uart::send(command.getBuffer(), command.getLength());
			lastRequestSent = util::getTime();
		}
	}

	if (callback) {
		callback(buffer);
	}
}

void motor::sleep(uint8_t address) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::Sleep, 0}
	};

	uart::send(command.getBuffer(), command.getLength());
}

void motor::idle(uint8_t address) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::Idle, 0}
	};

	uart::send(command.getBuffer(), command.getLength());
}

void motor::wake(uint8_t address) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::Wake, 0}
	};

	uart::send(command.getBuffer(), command.getLength());
}

void motor::move(uint8_t address, uint16_t position, uint8_t torque) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::Move, 2}
	};
	auto& buf {command.getBuffer()};

	if (util::getTime() - lastRequestSent > 50) {
		requestQueue.clear();
	}

	if (requestQueue.size()) {
		return;
	}

	buf[2] = (torque << 4u) | (position >> 8u);
	buf[3] = position;

	uart::send(command.getBuffer(), command.getLength());
}

void motor::tone(uint8_t address, uint16_t frequency) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::Tone, 2}
	};
	auto& buf {command.getBuffer()};

	buf[2] = frequency >> 8u;
	buf[3] = frequency;

	uart::send(command.getBuffer(), command.getLength());
}

void motor::haptic(uint8_t address, uint8_t intensity, uint16_t duration) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::Haptic, 2}
	};
	auto& buf {command.getBuffer()};

	buf[2] = (intensity << 4u) | (duration >> 8u);
	buf[3] = duration;

	uart::send(command.getBuffer(), command.getLength());
}

void motor::adjustOffset(uint8_t address, uint16_t targetPosition) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::AdjustOffset, 2}
	};
	auto& buf {command.getBuffer()};

	buf[2] = targetPosition >> 8u;
	buf[3] = targetPosition;

	uart::send(command.getBuffer(), command.getLength());
}

void motor::calibrate(uint8_t address, uint8_t mode) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::Calibrate, 1}
	};
	auto& buf {command.getBuffer()};

	buf[2] = mode;

	uart::send(command.getBuffer(), command.getLength());
}

void motor::getVariable(uint8_t address, MotorCommand::Variable variable) {
	auto command {
	  MotorCommand {0, address, MotorCommand::CommandType::GetVariable, 1}
	};
	auto& buf {command.getBuffer()};

	buf[2] = static_cast<uint8_t>(variable);

	if (util::getTime() - lastRequestSent > 50) {
		requestQueue.clear();
	}

	if (requestQueue.empty()) {
		uart::send(command.getBuffer(), command.getLength());
		lastRequestSent = util::getTime();
	}
	requestQueue.push_back(command);
}

void motor::send(const uint8_t* buf, uint8_t len, void (*cb)()) {
	uint8_t address = buf[0] & 0xf;

	if (!address) {  // Check destination address
		return;
	}

	uart::send(buf, len, cb);
}

void motor::setCallback(uart::DefaultCallback::callback_type cb) {
	uart::setCallback(processResponse);
	callback = cb;
}
