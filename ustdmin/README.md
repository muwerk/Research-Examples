# Minimal test project for the [ustd](https://github.com/muwerk/ustd) library with platformio

## Preparations

* [platformio](https://platformio.org/) needs to be installed.

## Build

Clone the project, and use

```bash
pio run
```

to build,

```bash
pio run -t upload
```

to install on a device connected via USB.

This should build the project for an atmega. Adapt `platformio.ini` for other hardware.

## References:

* [Platformio documentation](https://docs.platformio.org/en/latest/)
* [`ustd` library](https://github.com/muwerk/ustd)