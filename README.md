# Fiesta Online Packet Decoder

This project aims to reverse engineer Fiesta Online's custom network protocol in order to implement some other tools such as a DPS meter.

## Build instructions

This project contains submodules, there is should be cloned using:

```bash
git clone --recursive https://github.com/GetMoon2EZ/FiestaOnlinePacketDecoder
```

Go to the root directory then run with the correct parameters depending on you compiler:

```bash
# Compiling using Visual Studio generator
cmake -DMSVC=true -S . -B build -G "Visual Studio 16 2019" -A x64
cmake --build build
```

> 📝 CMake can be downloaded and installed from [here](https://cmake.org/download/)

## How to use

To use, run the executable file that is produced by the compilation under the `bin/Debug` folder.

## Currently implemented features

- [x] Print entity level, current & max HP/mana on entity selection in console (debug build only)
- [x] Display current DPS on UI
- [x] Display highest DPS in current FOPD session
- [x] Display latency with Fiesta Online server

## Work In Progress

- [ ] Display an accurate DPS of the player
