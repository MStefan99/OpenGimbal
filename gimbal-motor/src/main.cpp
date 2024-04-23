#include "main.hpp"


#define NVMTEMP ((uint32_t*)0x00806030)

static uint8_t maxTorque {0};

static MovementController movementController {};
static Mode mode {Mode::Calibrate};
static uint8_t calibrationMode = data::options.polePairs? 0 : 3;
static uint16_t rawTarget {0};

static uint16_t angle {0};
static bool dataReady {false};

static uint32_t hapticEnd {0};
static uint16_t hapticPower {0};
static uint8_t hapticDuration {0};
static bool hapticDirection {false};

auto kalman = Kalman<float, unsigned, 3, 1, 1> {x0, P0, Q, R};

constexpr float dt {1}; // In milliseconds, speed and velocity need to be multiplied by 1000
constexpr auto F = Matrix<float, unsigned, 3, 3> {{1, dt, dt * dt / 2},
                                        {0, 1, dt},
                                        {0, 0,  1}};
constexpr auto H = Matrix<float, unsigned, 1, 3> {{1, 0, 0}};

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
    
    switch(static_cast<Command::CommandType>(buffer.buffer[1] & 0x0f)) { // Switch command type
        case (Command::CommandType::Sleep): {
            mode = Mode::Sleep;
            break;
        }
        case (Command::CommandType::Position): {
            rawTarget = ((buffer.buffer[2] & 0x0f) << 8u) | buffer.buffer[3];
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
            movementController.adjustOffset(angle, ((buffer.buffer[2] & 0x0f) << 8u) | buffer.buffer[3]);
            int16_t offset = movementController.getOffset();
            data::edit(reinterpret_cast<const uint8_t&>(data::options.zeroOffset), reinterpret_cast<uint8_t&>(offset), sizeof(offset));
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
                    int16_t offset = movementController.getOffset();
                    data::edit(reinterpret_cast<const uint8_t&>(data::options.zeroOffset), reinterpret_cast<uint8_t&>(offset), sizeof(offset));
                    data::write();
                    break;
                }
                case (Command::Variable::Range): {
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
    uint16_t eAngleCW = (data::options.polePairs * (fullRevolution + angle - data::options.phaseOffset)) % fullRevolution;
    // Flip the angle if the motor polarity is reversed
    uint16_t eAngle = data::options.direction < 0? fullRevolution - eAngleCW : eAngleCW;
    
    bldc::applyTorque(counterclockwise == (data::options.direction > 0)? eAngle + 1024: eAngle + 3072, power);
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
    return static_cast<float>(counts) / fullRevolution * TWO_PI; 
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

    uart::init();
    dma::init();
    i2c::init();
    as5600::init();
    bldc::init();
    
    uart::setCallback(processCommand);
    movementController.setOffset(data::options.zeroOffset);
    movementController.setRange(data::options.range);
    
    LowPassFilter targetFilter {1000, 5};
    // TODO: Exiting out of no-load (proportional) mode is jerky and needs improvement
    LowPassFilter loadFilter {1000, 1};
    
    while (1) {
        switch(mode) {
            case (Mode::Sleep): {
                sleep();
                mode = Mode::Drive;
                break;
            }
            case (Mode::Calibrate): {
                calibrate();
                srand(SysTick->VAL);
                mode = Mode::Drive;
                break;
            }
            case (Mode::Drive): {
                #if DV_OUT
                    auto start = SysTick->VAL;
                #endif
                
                // Checking for target wraparound
                if (rawTarget - targetFilter.getState() > 2048) {
                    targetFilter.force(targetFilter.getState() + 4096);
                } else if (rawTarget - targetFilter.getState() < -2048) {
                    targetFilter.force(targetFilter.getState() - 4096);
                }
                movementController.setTarget(targetFilter.process(rawTarget));
                
                // Calculating difference between current and target angle
                angle = measureAngle();
                float dAngle = countsToRad(getDifference(movementController.getTarget(), angle));
                
                // Estimating state
                auto z = Matrix<float, unsigned, 1, 1> {{dAngle}};
                kalman.correct(H, z);
                auto kx {kalman.x()};
                
                // Calculating and applying torque
                auto x = Matrix<float, uint8_t, 2, 1> {{kx[0][0]}, {kx[1][0] * 1000}};
                
                if (util::abs(kx[2][0]) > switchAcceleration / 1000) {
                    loadFilter.force(0.0f);
                } else if (loadFilter.getState() < 1.0f) {
                    loadFilter.process(util::abs(dAngle) / switchSmoothness);
                }
                float torque = util::interpolate(dAngle * pGain, torque = (K * x)[0][0] * gain, loadFilter.getState());
                
                applyTorque(angle, util::min(static_cast<uint16_t>(util::abs(torque) + util::min(idleTorque, maxTorque)),
                    static_cast<uint16_t>(maxTorque)), torque > 0);
                
                // Predicting next state
                kalman.predict(F);
                
                util::runTasks();
                #if DV_OUT
                    if (util::getTime() % 5 < 1) {
                        Data data {};
                        
                        data.dt = (start - SysTick->VAL) / 48;
                        data.x = kx[0][0];
                        data.v = kx[1][0] * 1000;
                        data.a = kx[2][0] * 1000;
                        data.u = torque;
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
                
                util::sleep(1); // Waiting until next tick
                break;
            }
            case (Mode::Idle): {
                util::runTasks();
                // Fallthrough intentional
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
