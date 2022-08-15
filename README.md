# Fiesta Online Packet Decoder

This project aims to reverse engineer Fiesta Online's custom network protocol in order to implement some other tools such as a DPS meter.

## Dependencies

This project depends on:
* libtins

## Build instructions

Create a `build` folder under the `src` directory:

```
mkdir src/build
cmake -DMSVC=true/false ..
cmake --build .
```

## How to use

To use, run the executable file that is produced by the compilation under the `src/build/Debug` folder.
