# Software

Software part of the OpenGimbal project consists of the firmware for C1 and D1 and the Companion web app.
The firmware is made to work on its own - after you flash and calibrate the gimbal, you don't need any external devices -
you can turn the gimbal on/off and control it entirely using the buttons and joystick on the gimbal itself.
Companion app is useful for calibrating the gimbal or controlling it from a browser. It also contains the gimbal driver
itself (in /companion/src/scripts/driver directory) that you can use for making your own apps as long as your runtime
supports WebUSB.

## Development

If you can't use the provided driver, it is easy to control OpenGimbal from your hardware and programming language of choice.
Both C1 and D1 use a simple protocol and using it is as easy as sending a few bytes over UART.
Learn more [here](/protocols).
