# e-ink-with-kinetis
Explore how to use an e-ink display on a kinetis freedom board.

## Dependencies
1. arm-eabi-none-gcc toolchain
2. openocd (0.9.0 or newer)

## How to build

1. Attach e-ink devel board to your USB port.
2. Fire up openocd
```
openocd -f interface/cmsis-dap.cfg -f ./kx.cfg
```
3. Build and flash
```
make
make flash
```
