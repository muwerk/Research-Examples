## Switch and led example with Home Assistant support

A simple example demonstrating muwerk's scheduler, munet interface to WLAN, NTP, OTA and MQTT, and simple hardware (switch and led). Both led and switch autoregister as entities in Home Assistant using auto-discovery.

Home Assistant support requires just a single line of code per entity:

```cpp
    // Use Home Assistant's auto-discovery to register switch and led in HA
    toggleswitch.registerHomeAssistant("DigiTast");
    led.registerHomeAssistant("Blaue Led");
```

<img src="https://github.com/muwerk/mupplets/blob/master/Resources/switch.png" width="50%" height="30%">
Hardware: 330Ω resistor, led, switch.

## MQTT examples

If munet has been configured to connect to an MQTT server (e.g. mosquitto), then the hardware
can be accessed externally:

To set the switch into timer mode:
```bash
mosquitto_pub -h <hostname-of-mqtt-server> -t <hostname-of-board>/mySwitch/switch/mode/set -m "timer 1000"
```

Default-mode:
```bash
mosquitto_pub -h <hostname-of-mqtt-server> -t <hostname-of-board>/mySwitch/switch/mode/set -m "default"
```

Let led signal a pattern (e.g. POST code):
```bash
mosquitto_pub -h <hostname-of-mqtt-server> -t <hostname-of-board>/myLed/light/mode/set -m "pattern +-+-+-+++-+++-+++-+-+-+---r,100"
```

Subscribe to receive messages from the board about state changes:
```bash
mosquitto_sub -h <hostname-of-mqtt-server> -v -t omu/#

omu/testwemos/mySwitch/switch/state off
omu/testwemos/myLed/light/unitluminosity 0.0
omu/testwemos/myLed/light/state off
omu/testwemos/mySwitch/switch/state on
omu/testwemos/myLed/light/unitluminosity 1.0
omu/testwemos/myLed/light/state on
omu/testwemos/mySwitch/switch/state off
omu/testwemos/myLed/light/unitluminosity 0.0
omu/testwemos/myLed/light/state off
```

## Documentation

* [muwerk scheduler and messaging](https://github.com/muwerk/muwerk)
* [munet networking and mqtt](https://github.com/muwerk/munet)
* [documentation of hardware mupplets and supported MQTT messages](https://github.com/muwerk/mupplets)
* [Home Assistant's auto-discovery for MQTT](https://www.home-assistant.io/docs/mqtt/discovery/)