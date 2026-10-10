# OpenGimbal protocols

To control OpenGimbal, you simply need to send a few bytes over UART. This page describes how to do it.

## D1

D1 commands have three parts: a setup byte, a command byte and optional command-specific data.

### Setup byte

| LEN[3:0] | DSTADDR[3:0] |
| -------- | ------------ |

Setup byte is split into two halves. Most significant bits (LEN[3:0]) contain the number of the **remaining** bytes
(excluding setup byte!). Least significant bits contain the address of the destination board.

Each board (C1, D1) on the data line **must** have a unique address that are allocated as follows:

| Address   | Description          |
| --------- | -------------------- |
| 0x0       | Controller (C1)      |
| 0x1 - 0xE | Motor (D1)           |
| 0xF       | Motor broadcast (D1) |

Address 0 (0x0) is reserved for the controller, while address 15 (0xF) will apply to all drivers on the data line.
Otherwise, drivers are addressed individually by their address.

### Command byte

| SRCADDR[3:0] | CMD[3:0] |
| ------------ | -------- |

Command byte is split in two, similar to the setup byte. Most significant bits contain the address of the board that sent the
command (useful for knowing which board responded, for instance) while least significant bits contain the command code itself.
Currently, the following commands are supported:

| Code | Command                                 |
| ---- | --------------------------------------- |
| 0x0  | [Sleep](#sleep-command)                 |
| 0x1  | [Idle](#idle-command)                   |
| 0x2  | [Wake](#wake-command)                   |
| 0x3  | [Move](#move-command)                   |
| 0x4  | [Tone](#tone-command)                   |
| 0x5  | [Haptic](#haptic-command)               |
| 0x6  | [Adjust offset](#adjust-offset-command) |
| 0xE  | [Set variable](#set-variable-command)   |
| 0xF  | [Get variable](#get-variable-command)   |

### Variables

Drivers store certain information as variables that can be read and written. Variables supported by D1 are listed [here](#d1-variables).

### Command description

#### Sleep command

Instructs the driver to enter the deepest sleep mode. Rising edge on the data line will wake the driver. Not compatible with standart UART!

#### Idle command

Instructs the driver to enter sleep mode. Any command except Sleep and Idle will wake the driver.

#### Wake command

Dedicated command that tells the driver to wake from sleep mode. Any other command except Sleep and Idle will also wake the driver but
the Wake command is guaranteed to have no side effects.

### Move command

Instructs the driver to move the motor into a specified position. Accepts the following data:

| PWR[3:0] | POS[11:8] |
| -------- | --------- |

| POS[7:0] |
| -------- |

Arguments

- PWR: Motor torque. 0 is no torque, 15 is maximum
- POS: Target position

### Tone command

Changes the PWM frequency of the driver. Lower frequencies produce an audible tone, while retaining full motor control. Set to 25+ kHz to disable sound.

| FREQ[15:8] |
| ---------- |

| FREQ[7:0] |
| --------- |

Arguments

- FREQ: Tone frequency

### Haptic command

Generates haptic feedback, from single pings to longer vibration patterns.

| INTN[3:0] | DUR[11:8] |
| --------- | --------- |

| DUR[7:0] |
| -------- |

Arguments

- INTN: Haptic feedback intensity
- DUR: Duration in milliseconds

### Adjust offset command

Adjusts the offset of the driver so that the current (measured) offset becomes the target value. Useful for setting zero position.

| -   | OFFVAL[11:8] |
| --- | ------------ |

| OFFVAL[7:0] |
| ----------- |

Arguments

- OFFVAL: Offset value that will correspond to current position

### Set variable command

Instructs the driver to set the variable to the given value.

| VID[7:0] |
| -------- |

VID byte should be followed by one or more bytes containing the value of the variable.

Arguments:

- VID: ID of the variable to be written

### Get variable command

Instructs the driver to return the value of a given variable. Only works with individual driver addresses 0x1-0xe (address 0xf will be ignored).

The addressed driver will respond with the command following the same structure,
except for the swapped DSTADDR and SRCADDR.

| VID[7:0] |
| -------- |

Arguments:

- VID: ID of the variable to be read

### D1 variables

| VID  | Name     | Size | Mode | Persistent | Description            | Allowed values                                          | Comments                                                                |
| ---- | -------- | ---- | ---- | ---------- | ---------------------- | ------------------------------------------------------- | ----------------------------------------------------------------------- |
| 0x00 | Options  | 1    | R/W  | Yes        | Driver options         | Bit mask. Bit 0 - Calibration, bit 1 - invert direction | Writing 1 to the calibration bit will immediately start the calibration |
| 0x01 | Offset   | 2    | R/W  | Yes        | Logical motor offset   | 0-4096                                                  |                                                                         |
| 0x02 | Position | 2    | R    | No         | Current motor position |                                                         |                                                                         |

## C1

C1 commands are organized the same way as D1 commands. The setup byte in the case of C1 only contains the command ID. When using UART, the command needs to be prefixed with a byte containing the length of the **remaining** bytes (excluding the length byte itself!).

| LEN (UART only) | CMD | Command data bytes |
| --------------- | --- | ------------------ |

### C1 commands

| Code | Command           |
| ---- | ----------------- |
| 0x00 | Disable           |
| 0x01 | Enable            |
| 0x0D | Set variable      |
| 0x0E | Get variable      |
| 0x0F | Motor passthrough |

### Disable command

Disables C1 and power to the motors.

### Enable command

Enables C1 and power to the motors.

### Set/get variable

Writes and reads variables similar to D1. The list of C1 variables can be found [here](#c1-variables).

### Motor passthrough

Passes bytes following the command byte to the motors as is.

### C1 variables

| VID  | Name               | Size | Mode | Persistent                   | Description                                           | Allowed values                                  |
| ---- | ------------------ | ---- | ---- | ---------------------------- | ----------------------------------------------------- | ----------------------------------------------- |
| 0x00 | Orientation        | 6    | R/W  | No                           | Head orientation as Euler angles (yaw, pitch, roll)   | 3 signed 16-bit values. Scaling factor pi/32768 |
| 0x01 | Handle orientation | 6    | R    | No                           | Handle orientation as Euler angles (yaw, pitch, roll) | 3 signed 16-bit values. Scaling factor pi/32768 |
| 0x02 | Mode               | 1    | R/W  | [Gimbal mode](#gimbal-modes) | 0-3                                                   |
| 0x03 | Battery voltage    | R    | No   | 0-255                        |

### Gimbal modes

| ID   | Mode    | Comments                                                             |
| ---- | ------- | -------------------------------------------------------------------- |
| 0x00 | Horizon | Pitch and roll locked, only yaw follows the handle                   |
| 0x01 | Follow  | Roll locked, yaw and pitch follow the handle                         |
| 0x02 | FPV     | Yaw, pitch and roll follow the handle                                |
| 0x03 | Tilt    | Same as FPV, except the joystick controls camera roll instead of yaw |
