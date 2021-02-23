# ATMEGA2560_THR_INF_Firmware

Embedded codebase for the ATMEGA2560 MCU responsible for interfacing with the ESCs/Thrusters on VortexNTNU 'Gladlaks' AUV.

The main task of the MCU is to receive data from the main computer (Nvidia Xavier) on the AUV, process it, and relay it to the iFLight SucceX ESCs, which will then give signals to the T200 Thrusters accordingly.

Communication between the MCU and the computer happens using the I²C protocol.

Dependencies:
- AVR-GCC
- AVRDUDE
---
To compile:
```
$ cd ~/thruster_embedded
$ make
```

To flash:
```
$ cd ~/thruster_embedded
$ make
$ make install
```
