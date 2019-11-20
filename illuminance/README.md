## LDR luminosity measurment example 

A simple example demonstrating muwerk's scheduler, munet interface to WLAN, NTP, OTA and MQTT, LDR illuminance sensor and TSL2561 I2C lux sensor. 

<img src="https://github.com/muwerk/mupplets/blob/master/Resources/light.png" width="60%" height="30%">
Hardware: 10kΩ, LDR sensor, TSL2561.

* Please check [application notes](https://github.com/muwerk/mupplets#ldrh)

The example publishes luminosity changes to MQTT:

```bash
mosquitto_sub -h <mqtt-server-host> -t omu/# -v

# Output (two sensors, LDR (myLdr) and TSL2561 (myLum)):
omu/testesp32/myLum/sensor/unitilluminance 0.315
omu/testesp32/myLdr/sensor/unitilluminance 0.701
omu/testesp32/myLdr/sensor/unitilluminance 0.694
omu/testesp32/myLum/sensor/illuminance  246
omu/testesp32/myLum/sensor/unitilluminance 0.308
omu/testesp32/myLdr/sensor/unitilluminance 0.688
omu/testesp32/myLdr/sensor/unitilluminance 0.683
omu/testesp32/myLdr/sensor/unitilluminance 0.676
omu/testesp32/myLdr/sensor/unitilluminance 0.668
omu/testesp32/myLum/sensor/illuminance  239
omu/testesp32/myLum/sensor/unitilluminance 0.298
omu/testesp32/myLdr/sensor/unitilluminance 0.673
omu/testesp32/myLdr/sensor/unitilluminance 0.681
omu/testesp32/myLdr/sensor/unitilluminance 0.687
omu/testesp32/myLdr/sensor/unitilluminance 0.693
```

## Documentation

* [muwerk scheduler and messaging](https://github.com/muwerk/muwerk)
* [munet networking and mqtt](https://github.com/muwerk/munet)
* [documentation of hardware mupplets and supported MQTT messages](https://github.com/muwerk/mupplets)
