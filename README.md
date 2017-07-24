# Drinks Strorage State
![build badge](https://api.travis-ci.org/flipdot/drinks-storage-state.svg)

This repo contains the design files (not yet) and firmware for the hardware, which measures the state of our beverage storage site.

## Concept
Our beverage storage site consists of multiple stacks of crates, where each stack can only contain one type of bottles which are either all full or all empty.

Each of the stacks is standing on a special scale, which monitors how many crates are on the stack.
Those special scales contain ESP8266 and send their state to a central server. The data is processed at server site and forwarded to a graphana server for visualisation.

![a diagram showing the oveall architecture of the monitoring system](doc/architecture_diagram.png)

## Hardware
The Hardware is based on a board with 4 load cells attahed to it. The signals of the loadcells are amplified and digitalized by a HX711 load cell amplifier which is connected to an ESP8266 Board.

Currently the HX711 is hooked up to pins 12 & 14 (clock & data) of the ESP.

## Software
The software is written with the arduino framework and build using [platformio](http://platformio.org/).
To build the firmware and upload it to the ESP simply install platfomio and run `platformio run -t upload`
