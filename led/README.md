## Switch and led example

A simple example demonstrating muwerk's scheduler, munet interface to WLAN, NTP, OTA and MQTT, and simple hardware (switch and led).

<img src="https://github.com/muwerk/mupplets/blob/master/Resources/switch.png" width="50%" height="30%">
Hardware: 330Ω resistor, led, switch.

## MQTT examples

If munet has been configured to connect to an MQTT server (e.g. mosquitto), then the hardware
can be accessed externally:

To set the switch into timer mode:
```bash
mosquitto_pub -h <hostname-of-board> -t <hostname-of-board>/mySwitch/switch/mode/set -m "timer 1000"
```

Default-mode:
```bash
mosquitto_pub -h <hostname-of-board> -t <hostname-of-board>/mySwitch/switch/mode/set -m "default"
```

Subscribe to receive messages from the board about state changes:
```bash
mosquitto_sub -h <hostname-of-board> -v -t omu/#

omu/testwemos/mySwitch/switch/state off
omu/testwemos/myLed/led/unitluminosity 0.0
omu/testwemos/myLed/led/state off
omu/testwemos/mySwitch/switch/state on
omu/testwemos/myLed/led/unitluminosity 1.0
omu/testwemos/myLed/led/state on
omu/testwemos/mySwitch/switch/state off
omu/testwemos/myLed/led/unitluminosity 0.0
omu/testwemos/myLed/led/state off
```

## Documentation

* [muwerk scheduler and messaging](https://github.com/muwerk/muwerk)
* [munet networking and mqtt](https://github.com/muwerk/munet)
* [documentation of hardware mupplets and supported MQTT messages](https://github.com/muwerk/mupplets)
