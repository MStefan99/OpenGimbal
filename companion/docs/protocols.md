# OpenGimbal protocols

To control OpenGimbal, you simply need to send a few bytes over UART. This page describes how to do it.

## D1
D1 commands have three parts: a setup byte, a command byte and optional command-specific data.

### Setup byte
| LEN[3:0] | DSTADDR[3:0] |
| --- | --- |

Setup byte is split into two halves. Most significant bits (LEN[3:0]) contain the number of the **remaining** bytes 
(excluding setup byte!). Least significant bits contain the address of the destination board.

Each board (C1, D1) on the data line **must** have a unique address that are allocated as follows:

| Address | Description |
| --- | --- |
| 0x0 | Controller (C1) |
| 0x1 - 0xE | Motor (D1) |
| 0xF | Motor broadcast (D1) |

Address 0 (0x0) is reserved for the controller, while address 15 (0xF) will apply to all drivers on the data line. 
Otherwise, drivers are addressed individually by their address.

### Command byte

| SRCADDR[3:0] | CMD[3:0] |
| --- | --- |

Command byte is split in two, similar to the setup byte. Most significant bits contain the address of the board that sent the
command (useful for knowing which board responded, for instance) while least significant bits contain the command code itself.
Currently, the following commands are supported:

| Code | Command |
| --- | --- |
| 0x0 | [Sleep](#sleep-command) |
| 0x1 | [Idle](#idle-command) |
| 0x2 | [Wake](#wake-command) |
| 0x3 | [Move](#move-command) |
| 0x4 | Tone |
| 0x5 | Haptic |
| 0x6 | Adjust offset |
| 0x7 | Calibrate |
| 0xE | Get variable |
| 0xF | Set variable |


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
| --- | --- |

| POS[7:0] |
| --- |

Arguments
- PWR: Motor torque. 0 is no torque, 15 is maximum
- POS: Target position
