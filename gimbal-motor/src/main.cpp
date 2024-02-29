#include "main.hpp"


#define NVMTEMP ((uint32_t*)0x00806030)

// The total degrees for one full rotation
static constexpr uint16_t fullRotation {4096};
// Minimum torque to get the motor moving
static constexpr uint8_t idleTorque {150};
// Device address
static constexpr uint8_t deviceAddress {1};
// Maximum allowed torque
static uint8_t maxTorque {0};

static MovementController movementController {};
static Mode mode {Mode::Calibrate};
static uint8_t calibrationMode = data::options.polePairs? 0 : 3;

static uint16_t angle {0};
static bool dataReady {false};

struct TorqueLUT {
	uint16_t table[16];

	constexpr TorqueLUT() : table() {
        table[0] = 0;
        auto factor = (255.0 - idleTorque) / 15.0;
		for (auto i = 1; i < 16; ++i) {
			table[i] = std::floor(factor * i + idleTorque);
		}
	}
};

static constexpr auto torqueLUT = TorqueLUT();

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processCommand(const uart::DefaultCallback::buffer_type& buffer) {   
    uint8_t address = buffer.buffer[0] & 0x0f;
    if (address != deviceAddress && address != 0xf) {
        return; // Command intended for another device
    }
    
    switch(static_cast<CommandType>(buffer.buffer[1] & 0x0f)) { // Switch command type
        case (CommandType::Sleep): {
            mode = Mode::Sleep;
            break;
        }
        case (CommandType::Position): {
            movementController.setTarget(((buffer.buffer[2] & 0x0f) << 8u) | buffer.buffer[3]);
            maxTorque = torqueLUT.table[buffer.buffer[2] >> 4u];
            break;
        }
        case (CommandType::Tone): {
            bldc::tone((buffer.buffer[2] << 8u) | buffer.buffer[3]);
            break;
        }
        case (CommandType::Offset): {
            movementController.adjustOffset(angle, ((buffer.buffer[2] & 0x0f) << 8u) | buffer.buffer[3]);
            int16_t offset = movementController.getOffset();
            data::edit(reinterpret_cast<const uint8_t&>(data::options.zeroOffset), reinterpret_cast<uint8_t&>(offset), sizeof(offset));
            data::write();
            break;
        }
        case (CommandType::Calibrate): {
            mode = Mode::Calibrate;
            calibrationMode = buffer.buffer[2];
            break;
        }
        case (CommandType::GetVariable): {
            switch (static_cast<Variable>(buffer.buffer[2])) { // Switch variable
                case (Variable::Calibration): {
                    auto command = ReturnVariableCommand(deviceAddress, buffer.buffer[1] >> 8u,
                            static_cast<uint8_t>(Variable::Calibration), 
                            static_cast<uint8_t>((!!data::options.polePairs << 1u) | (!!data::options.phaseOffset)));
                    uart::send(command.getBuffer(), command.getLength());
                    break;
                } case (Variable::Offset): {
                    auto command = ReturnVariableCommand(deviceAddress, buffer.buffer[1] >> 8u,
                            static_cast<uint8_t>(Variable::Offset), 
                            static_cast<uint16_t>(movementController.getOffset()));
                    uart::send(command.getBuffer(), command.getLength());
                    break;
                } case (Variable::Range): {
                    auto command = ReturnVariableCommand(deviceAddress, buffer.buffer[1] >> 8u,
                            static_cast<uint8_t>(Variable::Range), 
                            movementController.getRange());
                    uart::send(command.getBuffer(), command.getLength());
                    break;
                }
            }
            break;
        }
        case (CommandType::SetVariable): {
            switch (static_cast<Variable>(buffer.buffer[2])) { // Switch variable
                case (Variable::Offset): {
                    movementController.setOffset((buffer.buffer[3] << 8u) | buffer.buffer[4]);
                    int16_t offset = movementController.getOffset();
                    data::edit(reinterpret_cast<const uint8_t&>(data::options.zeroOffset), reinterpret_cast<uint8_t&>(offset), sizeof(offset));
                    data::write();
                    break;
                }
                case (Variable::Range): {
                    movementController.setRange((buffer.buffer[3] << 8u) | buffer.buffer[4]);
                    int16_t range = movementController.getRange();
                    data::edit(reinterpret_cast<const uint8_t&>(data::options.range), reinterpret_cast<uint8_t&>(range), sizeof(range));
                    data::write();
                    break;
                }
            }
            break;
        }
    }
}

uint16_t measureAngle() {
    static uint16_t rawAngle;
    
    dataReady = false;
    auto startTime {util::getTime()};
    
    as5600::getAngle([](bool success, const dma::I2CTransfer& transfer) {
        rawAngle = reinterpret_cast<const uint16_t*>(transfer.buf)[0];
        dataReady = true;
    });
    
    while (!dataReady && util::getTime() - startTime < 5) {
        __WFI();
    }
    
    return util::switchEndianness(rawAngle);
}

void calibrate() {
    uint16_t angle {0};
    uint16_t phaseOffset {data::options.phaseOffset};
    bldc::applyTorque(0, 255);
    
    if (calibrationMode) {
        do {
            util::sleep(20);
            phaseOffset = angle;
            angle = measureAngle();
        } while (phaseOffset != angle);
        data::edit(reinterpret_cast<const uint8_t&>(data::options.phaseOffset), reinterpret_cast<uint8_t&>(phaseOffset), sizeof(phaseOffset));
    }
    
    uint16_t torqueAngle {0}; 
    
    if (calibrationMode & (1 << static_cast<uint8_t>(CalibrationMode::Pole))) {
        uint8_t polePairs {0};
        uint16_t lastPoleAngle {angle};
        int8_t direction {0};
    
        do {
            torqueAngle += 10;

            if (torqueAngle >= fullRotation) {
                torqueAngle = 0;
                if (angle > lastPoleAngle) {
                    ++direction;
                } else {
                    --direction;
                }
                lastPoleAngle = angle;
                ++polePairs;
            }
            bldc::applyTorque(torqueAngle, 255);

            angle = measureAngle();
        } while (util::abs(angle - phaseOffset) > 10 || polePairs == 0);
        
        if (direction > 0) {
            direction = 1;
        } else {
            direction = -1;
        }
        
        data::edit(data::options.polePairs, polePairs);
        data::edit(reinterpret_cast<const uint8_t&>(data::options.direction), reinterpret_cast<uint8_t&>(direction));
    }
    
    if (calibrationMode) {
        data::write();
    }
    
    bldc::applyTorque(0, 0);
}

int16_t getDifference(uint16_t angleA, uint16_t angleB) {
    int16_t diff = static_cast<int16_t>(angleA) - static_cast<int16_t>(angleB);
    
    if (diff > 2048) {
        return diff - 4096;
    } else if (diff < -2048) {
        return diff + 4096;
    } else {
        return diff;
    }
}

void applyTorque(uint16_t angle, uint8_t power, bool counterclockwise = true) {
    // Calculate electrical angle from encoder reading
    uint16_t eAngleCW = (data::options.polePairs * (fullRotation + angle - data::options.phaseOffset)) % fullRotation;
    // Flip the angle if the motor polarity is reversed
    uint16_t eAngle = data::options.direction < 0? fullRotation - eAngleCW : eAngleCW;
    
    bldc::applyTorque(counterclockwise == (data::options.direction > 0)? eAngle + 1024: eAngle + 3072, power);
}

void sleep() {
    bldc::applyTorque(0, 0);
    // Motor timers are double-buffered, meaning the outputs aren't disabled immediately
    // Waiting for the next tick as a workaround, this needs to be fixed later
    __WFI(); 
    PM_REGS->PM_SLEEPCFG = PM_SLEEPCFG_SLEEPMODE_STANDBY;
    while (PM_REGS->PM_SLEEPCFG != PM_SLEEPCFG_SLEEPMODE_STANDBY);
    __WFI();
    PM_REGS->PM_SLEEPCFG = PM_SLEEPCFG_SLEEPMODE_IDLE;
    while (PM_REGS->PM_SLEEPCFG != PM_SLEEPCFG_SLEEPMODE_IDLE);
}

int main() {
    util::init();

    uart::init();
    dma::init();
    i2c::init();
    as5600::init();
    bldc::init();
    
    uart::setCallback(processCommand);
    movementController.setOffset(data::options.zeroOffset);
    movementController.setRange(data::options.range);
    
    float v {0.0f};
    float a {0.0f};
    float torque {0.0f};
    
    while (1) {
        switch(mode) {
            case (Mode::Sleep): {
                sleep();
                mode = Mode::Drive;
                break;
            }
            case (Mode::Calibrate): {
                calibrate();
                mode = Mode::Drive;
                break;
            }
            case (Mode::Drive): {
                // Calculating difference between current and set angle
                int16_t dAngle = getDifference(movementController.getTarget(), angle);
                uint16_t prevAngle {angle};
                angle = measureAngle();

                // Updating current speed and acceleration
                float newV = v + (getDifference(angle, prevAngle) - v) / 30.0f;
                float newA {newV - v};
                v = newV;

                if (dAngle < -128 || dAngle > 128) { // Far from target, using time-optimal control
                    float D = v * v - 2.0f * a * dAngle; // Calculate if the motor will reach the target (>=0 - yes, <0 - no)

                    if (D <= 2) { // Update measured acceleration value during acceleration
                        a += (newA - a) / 20.0f;
                    }
                    if (dAngle > 0 == a < 0) { // To avoid chatter and incorrect measurements, flip acceleration sign if needed
                        a = -a;
                    }

                    if (-0.005 < a && a < 0.005) { // Acceleration isn't measured yet, applying full power to measure
                        applyTorque(angle, maxTorque, dAngle > 0); // Accelerating towards destination
                    } else if (util::abs(D) > 2) { // Acceleration measured, using it to predict the stopping distance
                        applyTorque(angle, maxTorque, dAngle > 0 == D <= 0); // Accelerating or decelerating based on prediction
                    }
                } else { // Close to target, using sliding mode control
                    a = 0.0f; // Resetting acceleration for time-optimal initial measurement
                    torque += ((dAngle - 100 * v) - torque) / 10.0f; // Keeping velocity equal to dAngle / 100
                    applyTorque(angle, util::min(static_cast<int16_t>(util::abs(torque) + 140), static_cast<int16_t>(maxTorque)), torque > 0);
                }

                util::runTasks();
                util::sleep(1);
                break;
            }
        }
    }

    return 1;
}
