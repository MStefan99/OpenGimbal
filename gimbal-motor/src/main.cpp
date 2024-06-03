#include "main.hpp"


#define NVMTEMP ((uint32_t*)0x00806030)

static uint8_t maxTorque {0};
static constexpr uint16_t halfRevolution {fullRevolution / 2};
static constexpr uint16_t quarterRevolution {fullRevolution / 4};

static MovementController movementController {};
static Mode mode {Mode::Calibrate};
static uint8_t calibrationMode = data::options.polePairs? 0 : 3;
static uint32_t lastTargetTime {0};

static LowPassFilter angleFilter {1000, 5};
static bool dataReady {false};

static uint32_t hapticEnd {0};
static uint16_t hapticPower {0};
static uint8_t hapticDuration {0};
static bool hapticDirection {false};

struct TorqueLUT {
	uint16_t table[16];

	constexpr TorqueLUT() : table() {
        table[0] = 0;
        auto factor = (255.0f - idleTorque) / 15.0f;
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
    
    if (mode == Mode::Sleep) {
        mode = Mode::Drive; // Exit the sleep mode if we're the target device
    }
    
    switch(static_cast<Command::CommandType>(buffer.buffer[1] & 0x0f)) { // Switch command type
        case (Command::CommandType::Sleep): {
            mode = Mode::Sleep;
            break;
        }
        case (Command::CommandType::Position): {
            auto time {util::getTime()};
            movementController.extrapolate(time - lastTargetTime, ((buffer.buffer[2] & 0x0f) << 8u) | buffer.buffer[3]);
            lastTargetTime = time;
            maxTorque = torqueLUT.table[buffer.buffer[2] >> 4u];
            break;
        }
        case (Command::CommandType::Tone): {
            bldc::tone((buffer.buffer[2] << 8u) | buffer.buffer[3]);
            break;
        }
        case (Command::CommandType::Haptic): {
            mode = Mode::Haptic;
            hapticPower = buffer.buffer[2];
            hapticEnd = util::getTime() + buffer.buffer[3];
            break;
        }
        case (Command::CommandType::Offset): {
            movementController.adjustOffset(angleFilter.getState(), ((buffer.buffer[2] & 0x0f) << 8u) | buffer.buffer[3]);
            uint16_t offset = movementController.getOffset();
            data::edit(&data::options.zeroOffset, offset);
            data::write();
            break;
        }
        case (Command::CommandType::Calibrate): {
            mode = Mode::Calibrate;
            calibrationMode = buffer.buffer[2];
            break;
        }
        case (Command::CommandType::GetVariable): {
            if (address == 0xf) { // Get variable commands cannot be issued to all devices at once
                break;
            }
            switch (static_cast<Command::Variable>(buffer.buffer[2])) { // Switch variable
                case (Command::Variable::Calibration): {
                    auto command = ReturnVariableCommand(deviceAddress, buffer.buffer[1] >> 8u,
                            Command::Variable::Calibration, 
                            static_cast<uint8_t>((!!data::options.polePairs << 1u) | (!!data::options.phaseOffset)));
                    uart::send(command.getBuffer(), command.getLength());
                    break;
                } case (Command::Variable::Offset): {
                    auto command = ReturnVariableCommand(deviceAddress, buffer.buffer[1] >> 8u,
                            Command::Variable::Offset, 
                            static_cast<uint16_t>(movementController.getOffset()));
                    uart::send(command.getBuffer(), command.getLength());
                    break;
                } case (Command::Variable::Range): {
                    auto command = ReturnVariableCommand(deviceAddress, buffer.buffer[1] >> 8u,
                            Command::Variable::Range, 
                            movementController.getRange());
                    uart::send(command.getBuffer(), command.getLength());
                    break;
                }
            }
            break;
        }
        case (Command::CommandType::SetVariable): {
            switch (static_cast<Command::Variable>(buffer.buffer[2])) { // Switch variable
                case (Command::Variable::Offset): {
                    movementController.setOffset((buffer.buffer[3] << 8u) | buffer.buffer[4]);
                    uint16_t offset = movementController.getOffset();
                    data::edit(&data::options.zeroOffset, offset);
                    data::write();
                    break;
                }
                case (Command::Variable::Range): {
                    movementController.setRange((buffer.buffer[3] << 8u) | buffer.buffer[4]);
                    data::edit(&data::options.range, movementController.getRange());
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
    
    AS5600::getAngle([](bool success, const dma::I2CTransfer& transfer) {
        rawAngle = reinterpret_cast<const uint16_t*>(transfer.buf)[0];
        dataReady = true;
    });
    
    while (!dataReady && util::getTime() - startTime < 5) {
        __WFI();
    }
    
    return util::switchEndianness(rawAngle);
}

int16_t getDifference(uint16_t angleA, uint16_t angleB = 0) {
    int16_t diff = static_cast<int16_t>(angleA) - static_cast<int16_t>(angleB);
    
    if (diff > halfRevolution) {
        return diff - fullRevolution;
    } else if (diff < -halfRevolution) {
        return diff + fullRevolution;
    } else {
        return diff;
    }
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
        data::edit(&data::options.phaseOffset, phaseOffset);
    }
    
    uint16_t torqueAngle {0}; 
    
    if (calibrationMode & (1 << static_cast<uint8_t>(Command::CalibrationMode::Pole))) {
        uint8_t polePairs {0};
        uint16_t lastPoleAngle {angle};
        int8_t direction {0};
    
        do {
            torqueAngle += 10;

            if (torqueAngle >= fullRevolution) {
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
            util::sleep(1);
        } while (util::abs(angle - phaseOffset) > 10 || !polePairs);
        
        // Adding the last pole if we're close
        polePairs += torqueAngle / halfRevolution;
        
        // Normalizing direction
        direction = util::sign(direction);
        
        // Saving calibration
        data::edit(&data::options.polePairs, polePairs);
        data::edit(&data::options.direction, direction);
    }
    
    if (calibrationMode) {
        data::write();
    }

    bldc::applyTorque(0, 0);
}

void applyTorque(uint16_t angle, uint8_t power, bool counterclockwise = true) {
    // Calculate electrical angle from encoder reading
    uint16_t eAngleCW = (data::options.polePairs * (fullRevolution + angle - data::options.phaseOffset)) % fullRevolution;
    // Flip the angle if the motor polarity is reversed
    uint16_t eAngle = data::options.direction < 0? fullRevolution - eAngleCW : eAngleCW;
    
    bldc::applyTorque(counterclockwise == (data::options.direction > 0)? eAngle + quarterRevolution: eAngle + (fullRevolution - quarterRevolution), power);
}

void sleep() {
    bldc::applyTorque(0, 0);
    // Motor timers are double-buffered, meaning the outputs aren't disabled immediately
    // Waiting for the next tick as a workaround, this needs to be fixed later
    util::sleep(1);
    PM_REGS->PM_SLEEPCFG = PM_SLEEPCFG_SLEEPMODE_STANDBY;
    while (PM_REGS->PM_SLEEPCFG != PM_SLEEPCFG_SLEEPMODE_STANDBY);
    __WFI();
    PM_REGS->PM_SLEEPCFG = PM_SLEEPCFG_SLEEPMODE_IDLE;
    while (PM_REGS->PM_SLEEPCFG != PM_SLEEPCFG_SLEEPMODE_IDLE);
}

float countsToRad(int16_t counts) {
    return static_cast<float>(counts) / fullRevolution * F_2_PI; 
}

#if DV_OUT
struct Data {        
    uint8_t header {0x03};
    uint16_t dt {};
    float x {};
    float v {};
    float a {};
    float u {};
    uint8_t footer {0xfc};
} __attribute__((packed));
#endif

int main() {
    util::init();
    
    PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_HWSEL(1)
            | PORT_WRCONFIG_PINMASK((0x1 << 14u) | (0x1 << 15u))
            | PORT_WRCONFIG_PMUXEN(0)
            | PORT_WRCONFIG_WRPMUX(1)
            | PORT_WRCONFIG_WRPINCFG(1);

    uart::init();
    dma::init();
    i2c::init();
    AS5600::init();
    bldc::init();
    
    uart::setCallback(processCommand);
    
    LowPassFilter torqueFilter {1000, 2};
    float prevDAngle {0.0f};
    
    while (1) {
        switch(mode) {
            case (Mode::Sleep): {
                sleep();
                break;
            }
            case (Mode::Calibrate): {
                calibrate();
                mode = Mode::Drive;
                break;
            }
            case (Mode::Drive): {
                #if DV_OUT
                    auto start = SysTick->VAL;
                #endif
                
                auto time {util::getTime()};
                movementController.interpolate(time - lastTargetTime);
                
                // Calculating difference between current and target angle
                auto angle {measureAngle()};
                angleFilter.process(angle);
                
                float dAngle {countsToRad(getDifference(movementController.getTarget(), angle))};
                float velocity {(dAngle - prevDAngle) * 1000.0f};
                prevDAngle = countsToRad(getDifference(movementController.getTarget(), angle));
                
                // Calculating and applying torque
                float torque = torqueFilter.process(dAngle * K[0][0] + velocity * K[0][1]);
                float absTorque = util::min(static_cast<uint16_t>(util::abs(torque) + util::min(idleTorque, maxTorque)),
                    static_cast<uint16_t>(maxTorque));
                
                applyTorque(angle, absTorque, torque > 0);
                
                util::runTasks();
                
                #if DV_OUT
                    if (time % 5 < 1) {
                        Data data {};
                        
                        data.dt = (start - SysTick->VAL) / 48;
                        data.x = dAngle;
                        data.v = velocity;
                        data.u = util::sign(torque) * absTorque;
                        uart::send(reinterpret_cast<uint8_t*>(&data), sizeof(data));
                    }
                #endif
                
                util::sleep(1); // Waiting until next tick
                break;
            }
            case (Mode::Haptic): {
                if (hapticDuration++ > hapticCycleDuration) {
                    hapticDuration = 0;
                    hapticDirection = !hapticDirection;
                }
                
                applyTorque(measureAngle(), hapticPower, hapticDirection);
                
                if (util::getTime() > hapticEnd) {
                    mode = Mode::Drive;
                    hapticDuration = 0;
                }
                
                util::runTasks();
                util::sleep(1); // Waiting until next tick
                break;
            }
            case (Mode::Idle): {
                util::runTasks();
                __WFI();
                break;
            }
            default:
            {
                __WFI();
                break;
            }
        }
    }

    return 1;
}
