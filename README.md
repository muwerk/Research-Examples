**Note:** This project will be superseeded by a new example project, that will rely on the new [WIP] [mupplet-core](https://github.com/muwerk/mupplet-core) library. Currently both this and the `mupplet-core` are unfinished and under
active development. 

These Reserach-examples rely on the yet **unfinished** (and not yet published to the Arduino library repository) project [Research-mupplets](https://github.com/muwerk/Research-mupplets). To use the examples, simply manually add the Research-mupplet `*.h` files to the example projects. 

# Research-Examples [WIP]

Examples and test code for muwerk libraries ustd, munet, muwerk

* Simple [led and switch example](https://github.com/muwerk/Examples/tree/master/led) demonstrating muwerk scheduler, 
munet MQTT interface and simple led and button hardware interaction.
* Temperature and humidity measurement via DHT 22, DHT 11 or DHT 21. Results are published via MQTT. Automatic exponential filtering of sensor-data: [dht example](https://github.com/muwerk/Examples/tree/master/dht)
* Servo and PWM via I2C-PWM board, servo control via MQTT: [servo example](https://github.com/muwerk/Examples/tree/master/servo)

## Documentation

See [Application notes](https://github.com/muwerk/mupplets/blob/master/README.md) for more documentation.

* [muwerk scheduler and messaging](https://github.com/muwerk/muwerk)
* [munet networking and mqtt](https://github.com/muwerk/munet)
* [hardware mupplets and supported MQTT messages](https://github.com/muwerk/mupplets)
