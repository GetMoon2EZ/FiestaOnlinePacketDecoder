# Fiesta Online Packet Decoder

This project aims to reverse engineer Fiesta Online's custom network protocol in order to implement some other tools such as a DPS meter.

## Dependencies

This project depends on:
* libtins

## Build instructions

Go to the root directory then run with the correct parameters depending on you compiler:

```
cmake -DMSVC=true -S . -B build -G "Visual Studio 16 2019" -A x64
cmake --build build
```

## How to use

To use, run the executable file that is produced by the compilation under the `build/src/Debug` folder.

## Currently implemented features

- [x] Print entity level, current & max HP/mana on entity selection

## Work In Progress

- [x] Display an accurage DPS of the player
