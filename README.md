OpenGimbal
==========

*A modular brushless gimbal platform*

Overview
--------

* **Modularity**: Each board is designed to be modular, allowing you to replace or integrate them into other projects as needed. This modularity also enables users to customize their gimbal setup.
* **Customizability**: With well-defined UART APIs, each part can be controlled independently, giving you the flexibility to tailor your gimbal's behavior to specific applications.


Key Features
------------

* **Modularity**: Each board is designed to be modular, allowing you to replace or integrate them into other projects as needed. This modularity also enables users to customize their gimbal setup.
* **Customizability**: With well-defined UART APIs, each part can be controlled independently, giving you the flexibility to tailor your gimbal's behavior to specific applications.
* **Ease of Use**: Simple settings and built-in calibrations eliminate the need for manual adjustments, making it easy to get started with OpenGimbal.


Motor Controller Board
----------------------

* **Precise Control**: From a complete stop to thousands of RPM (depending on the motor), the board features sinusoidal control for smooth and accurate movements.
* **Torque Control**: The movement will be as gentle or strong as the motor allows, giving you unparalleled precision and control.
* **Feedback**: Use the motors themselves to generate tones and precise haptic feedback, providing extra degrees of feedback for a more immersive experience.


IMU (Inertial Measurement Unit) Board
-------------------------------------

The IMU Board measures the gimbal's orientation and controls multiple motors for stabilization. This board is designed to be highly accurate, ensuring smooth and stable video capture.


User Interface Board
--------------------

The User Interface Board allows users to interact with the gimbal through a simple and intuitive interface. With well-defined UART APIs, you can control the gimbal from any device that supports serial communication.


CAD Models and 3D Printed Gimbal Models
---------------------------------------

* **Customizable CAD Models**: CAD models are fully customizable, allowing you to tailor them to your specific needs.
* **3D Printed Gimbal Models**: Included 3D printed gimbal models can be assembled and customized as needed.


MATLAB Script for Inverse Kinematics
------------------------------------

For users who want to create custom gimbal geometries, an included MATLAB script can derive the inverse kinematics equations. This allows you to simulate and optimize your gimbal's performance before building it.


Compatibility
-------------

OpenGimbal is designed to support a wide range of cameras and smartphones, including:

* Any smartphone or camera weighing up to 250 grams (with a custom mount)
* Heavier cameras by upgrading the motors and plastic arms with stronger variants


Power and Battery Life
----------------------

OpenGimbal runs on two 18650 batteries, providing an impressive battery life:

* 24 hours of average continuous use
* Several months in standby mode


Firmware and Software
---------------------

The OpenGimbal firmware is written in C and C++, with no third-party libraries used. The companion web app is made with TypeScript and Vue, just needing a browser with WebSerial and WebUSB support.


Get Started
-----------

With OpenGimbal, you can start building your own custom gimbal today! Whether you're a seasoned developer or just starting out, our open-source project provides everything you need to get started.
