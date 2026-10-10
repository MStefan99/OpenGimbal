# Hardware

OpenGimbal needs a few parts to work: motors, controller boards plus some parts to connect and hold everything together. Let's cover all of these separately.

## C1

OpenGimbal C1 is the brain of the whole operation: it looks at its orientation in space, does some math and tells other boards what to do.
It also provides power to the gimbal from a Li-Ion battery and takes care of charging. C1 can be easily connected to a computer via USB or UART (Serial),
allowing you to control it either via OpenGimbal Companion (OpenGimbal's web interface, more on it [here](/software#companion))
or your custom software. You can control OpenGimbal from Arduino, Raspberry Pi, your smartphone or anything else with USB or UART
as long as it can run custom software. The only thing to keep in mind is that C1's UART is capped at 3.3 volts. Anything above
might **irreversibly** destroy the board.

## D1

D1 is a "dumb" board in a way - in a sense that it does nothing on its own. It is essentially a motor controller (inverter) -
you mount a D1 to each of the motors in your gimbal, tell them what to do and they will do exactly that. Need your motor to move into a certain position?
Make a sound? Signal something through vibration? Just tell D1 and it'll do the job. D1 is controlled over a bidirecional UART (single wire for Tx and Rx)
and its protocol allows you to run up to 14 motors from a single pin! D1 is fully compatible with standart UART by connecting Tx and Rx together
(except for the ultra-power saving mode during which the voltage should remain low). This means you can control up to 14 D1s from an Arduino, Raspberry Pi
or anything else with a UART - even your computer or phone through OpenGimbal Companion! The same 3.3 volt limit applies here too though.
The best way to do so is, of course, the C1. It already includes the firmware with all the heavy math, handles power seamlessly and is easy to use as-is
or connect to a computer.

## Motors

D1 will work with any 3-phase brushless DC motors, though motors made for gimbals would be ideal. The main limit here is motor current -
D1 can supply up to 2 amps of current at 11 volts (limited by the TMC6300) so pick motors with appropriate resistance. C1 supplies
10 volts to allow for voltage spikes.

## Part list

For one gimbal, you will need:

- C1
- D1 (x3)
- iFlight GBM2804H-100T\*
- 18650 Li-Ion cell
- Wires
- 3D printed parts
- M3 screws and nuts

\* Not sponsored. Other motors will work (see motor section above) but will require adjustments to the PCBs and/or 3D printed parts.
