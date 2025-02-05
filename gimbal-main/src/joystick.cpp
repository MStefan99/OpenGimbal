#include "joystick.hpp"

constexpr int deadzoneSquare = joystick::deadzone * joystick::deadzone;

static uint16_t        values[2] {};
static nvm::AxisPoints axisPoints[2] {};

static void (*callback)(int16_t x, int16_t y);

void joystick::update(void (*cb)(int16_t, int16_t)) {
	callback = cb;

	adc::getX([](uint16_t x) {
		values[0] = x;

		adc::getY([](uint16_t y) {
			values[1] = y;

			for (uint8_t i {0}; i < 2; ++i) {
				if (values[i] < axisPoints[i].min) {
					axisPoints[i].min = values[i];
				} else if (values[i] > axisPoints[i].max) {
					axisPoints[i].max = values[i];
				}
			}

			if (callback) {
				callback(getAxis(0), getAxis(1));
			}
		});
	});
}

int16_t joystick::getAxis(uint8_t axis) {
	axis %= 2;

	int16_t dx = values[0] - axisPoints[0].center;
	int16_t dy = values[1] - axisPoints[1].center;

	if (dx * dx + dy * dy < deadzoneSquare) {
		return 0;
	} else if (values[axis] < axisPoints[axis].center) {
		return util::scale<int16_t>(values[axis], axisPoints[axis].min, axisPoints[axis].center, -1000, 0);
	} else {
		return util::scale<int16_t>(values[axis], axisPoints[axis].center, axisPoints[axis].max, 0, 1000);
	}
}

void joystick::updateCenter() {
	update([](int16_t x, int16_t y) {
		for (uint8_t i {0}; i < 2; ++i) {
			if (!axisPoints[i].center) {
				axisPoints[i].center = values[i];
			} else {
				axisPoints[i].center = (axisPoints[i].center + values[i]) / 2;
			}
		}
	});
}

void joystick::saveValues() {
	//	nvm::edit(&nvm::options->xAxisPoints, xAxisPoints);
	//	nvm::edit(&nvm::options->yAxisPoints, yAxisPoints);
	//	nvm::write();
}
