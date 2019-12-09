## Darlington switch with 74LS595 and ULN2003

This demonstrates the shift_reg_74595 mupplet which controls a 74HC595 shift register using either SPI or bit-banging (with `shiftOut`).

<img src="https://github.com/muwerk/mupplets/blob/master/Resources/74hc595.png" width="60%" height="30%">
Hardware: 74HC595 shift register and (optional) 7-line darlington ULN2003.

7 bits of the 8 bit of 74HC595 are connected to the seven inputs of ULN2003, which can switch higher voltages and higher currents than the 74HC595. Use the ULN2803, if 8 outputs are required.

<img src="https://github.com/muwerk/mupplets/blob/master/Resources/74595-spi-pulseview.png" width="60%" height="60%">
Pulse view timing demonstrating SPI/bit bang input (first three lines) and 3 of 8 outputs (Q1-Q3 out of Q0..Q7).

A logic analyser with sigrok's [pulseview](https://sigrok.org/wiki/PulseView) software demonstrates the timing:

| PulseView diagram | Connection | Explanation
| ================= | ========== | ===========
| MOSI              | ESP D7 -> 74595 DS Pin 14    | Serial data output from ESP8266 to 74HC595
| SS                | ESP D8 -> 74595 ST_CP Pin 12 | Latch, if low, shift register can be reprogrammed without changing the output. On change to high, outputs (Q1-Q3) are updated with new value.
| SCK               | ESP D5 -> 74595 SH_CP Pin 11 | Serial clock
| Q1                | 74595 -> output 1 | 2. bit output (first bit is unused in this example)
| Q2                | 74595 -> output 2 | 3. bit output
| Q3                | 74595 -> output 3 | 4. bit output
