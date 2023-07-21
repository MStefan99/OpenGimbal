# Flight computer 

![Maintenance](https://img.shields.io/maintenance/no/2023?style=flat-square)



The goal of this project was to create a device that could autonomously control a model aircraft or drone
using the onboard inertial sensors and GPS. This repository contains the module firmware. At the current stage
all the basic functionality for stability assist is done, including various asynchonous drivers for communicating
with other onboard hardware such as sensors, other microcontrollers and computers over USB. The Kalman filter 
used for attitude estimation is also done along with PID controllers. However, the firmware development is currently
on hold until the current version is thoroughly tested and new hardware revision is made, at which point different firmware
repositories should be merged and hardware design files should be released.

