# Pico Pong

A very simple Pong for the [Raspberry Pi Pico](https://www.raspberrypi.org/documentation/pico/getting-started/)
and [Pimoroni Pico Display](https://shop.pimoroni.com/products/pico-display-pack), based on the
[Pimoroni Pico Libraries and Examples](https://github.com/pimoroni/pimoroni-pico).

## Build

1. Clone this repository
2. `mkdir build`
3. `cd build`
3. `export PICO_SDK_PATH=<path to Pico C/C++ SDK>`
4. `cmake ..`
5. `make`
6. Connect Pico with BootSel pressed; it will mount as a mass storage device
7. Copy `pong.uf2` to Pico
