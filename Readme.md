# nrf52_mynewt

nRF52 programming using Apache Mynewt OS and Nimble BLE stack

## Installation

Download 'newt' binary from https://mynewt.apache.org/download/ and make this avaliable to your $PATH

Also, make sure you download neccessary submodules with:

	git submodule update --recursive

## Debuggger

You can use a PICO board as CMSIS-DAP compatible debugger with the following firmware: https://github.com/raspberrypi/picoprobe/releases/download/picoprobe-cmsis-v1.02/debugprobe.uf2

## Programming

* First make sure you open a openocd server using `./scripts/cmsis_server.sh` script in separate terminal window.
* Then, go to `./firmware` folder and run `./iterate.sh main_release` to build the firmware and upload to host device. This also opens an active GDB connection to host device so you can use single stepping, breakpoints etc.

## Hardware interface

Even though Mynewt OS has some form of portable hardware abstraction library, it is not %100 comprehensive. If you don't need the portability, using lower level drivers gives you more control and can be even easier to develop for.

For nRF52 you can use https://github.com/NordicSemiconductor/nrfx, https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrfx/api_reference.html which is included with the Mynewt OS.

## Example Firmware

* Blinks an LED (P0.26) with 500ms period using Mynewt OS multitask API.
* Enables UART (RX=P1.12, TX=P1.11) with 115200 baud rate and print messages with [xprintf] interface.
* Initialize the BLE peripheral and start advertising.
	- There are lots to be done with Bluetooth Low Energy. See https://mynewt.apache.org/latest/network/ for detailed documentation and https://github.com/apache/mynewt-nimble/tree/master/apps/peripheral, https://github.com/apache/mynewt-nimble/tree/master/apps/bleprph examples.

[xprintf]: http://elm-chan.org/fsw/strf/xprintf.html
