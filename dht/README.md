## Temperature and humidity with DHT11, DHT21, or DHT22 example

A simple example demonstrating muwerk's scheduler, munet interface to WLAN, NTP, OTA and MQTT, and a DHT11, DHT21 or DHT22 temperature and humidity sensor.

<img src="https://github.com/muwerk/mupplets/blob/master/Resources/dht.png" width="30%" height="30%">
Hardware: 10kΩ, DHT22 sensor.

The example publishes temperature and humidity changes to MQTT:

```bash
mosquitto_sub -h <mqtt-server-host> -t omu/# -v

# Output:
omu/testwemos/myDht/humidity  63.2
omu/testwemos/myDht/humidity  61.8
omu/testwemos/myDht/humidity  60.7
omu/testwemos/myDht/humidity  58.9
omu/testwemos/myDht/humidity  57.7
omu/testwemos/net/rssi {"rssi":-55}
omu/testwemos/myDht/humidity  56.5
omu/testwemos/myDht/humidity  55.4
```

## Documentation

* [muwerk scheduler and messaging](https://github.com/muwerk/muwerk)
* [munet networking and mqtt](https://github.com/muwerk/munet)
* [documentation of hardware mupplets and supported MQTT messages](https://github.com/muwerk/mupplets)
