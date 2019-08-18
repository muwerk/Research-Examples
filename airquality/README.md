## Temperature and humidity, air pressure, air-quality (CO2, VOC) with DHT22, BMP085/BMP180 and CCS811 example

A simple example demonstrating muwerk's scheduler, munet interface to WLAN, NTP, OTA and MQTT, and a CCS811 air quality and DHT22 temperature and humidity sensor.

<img src="https://github.com/muwerk/mupplets/blob/master/Resources/airqual.png" width="70%" height="30%">
Hardware: CCS811 sensor, 10kΩ, DHT22 sensor, BMP180 sensor.

The example publishes air quality (CO2 and VOC levels), pressure, temperature and humidity changes to MQTT:

```bash
mosquitto_sub -h <mqtt-server-host> -t omu/# -v

# Output:
omu/testwemos/myDht/humidity  63.2
omu/testwemos/myDht/temperature  24.7
omu/testwemos/net/rssi {"rssi":-55}
omu/testwemos/myAirQuality/co2 417.5
omu/testwemos/myAirQuality/voc   2.2
omu/testwemos/myAirQuality/co2 415.4
omu/testwemos/myAirQuality/voc   1.9
omu/testwemos/myPressure/pressure 949.8
omu/testwemos/myDht/humidity  48.9
omu/testwemos/myAirQuality/voc   1.6
omu/testwemos/myAirQuality/voc   1.8
omu/testwemos/myAirQuality/co2 414.1
```

## Documentation

* [muwerk scheduler and messaging](https://github.com/muwerk/muwerk)
* [munet networking and mqtt](https://github.com/muwerk/munet)
* [documentation of hardware mupplets and supported MQTT messages](https://github.com/muwerk/mupplets)
