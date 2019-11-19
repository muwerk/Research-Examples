## LDR luminosity measurment example 

A simple example demonstrating muwerk's scheduler, munet interface to WLAN, NTP, OTA and MQTT, and a LDR light senors. 

<img src="https://github.com/muwerk/mupplets/blob/master/Resources/ldr.png" width="30%" height="30%">
Hardware: 10kΩ, LDR sensor.

* Please check [application notes](https://github.com/muwerk/mupplets#ldrh)

The example publishes luminosity changes to MQTT:

```bash
mosquitto_sub -h <mqtt-server-host> -t omu/# -v

# Output:
```

## Documentation

* [muwerk scheduler and messaging](https://github.com/muwerk/muwerk)
* [munet networking and mqtt](https://github.com/muwerk/munet)
* [documentation of hardware mupplets and supported MQTT messages](https://github.com/muwerk/mupplets)
