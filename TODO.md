# TODO

## Features

- [x] Plot damage over time
- [x] Add server selection (only Cypian is supported)
- [x] Add a reset button to reset plots/stats
- [ ] Customize default GUI
- [ ] Turn the app into an overlay
- [ ] Save stats to a JSON file
- [ ] Load stats from a JSON file
- [ ] Allow character selection for stats
- [ ] Only calculate average DPS during fights

## Bugs

- [x] Fix important memory leak (crash after a few minutes)
- [x] Fix a bug where an incredibly high damage is recorded (1e+09)
- [ ] Find a way to distinguish damage done and damage received

## Build

- [x] Make IMGUI a submodule
- [x] Improve build system
- [x] Fix vendor directory (missing dependencies not building with the project)
- [ ] Package the app using CMake/CPack
- [ ] Add support for x86 build

## MISC

- [ ] Add unit tests

## Proposed features

- [ ] Plot damage during donjon (if possible display a recap at the end)
- [ ] Add detailed logs of analyzed data in the GUI
- [ ] Add target HP on click in the GUI

## Precisions

As of right now, the DPS is calculated using every damage packet that is received by the client. Some of these packets represent damage done by an enemy to the player. Those packets should somehow be distinguished from the player's emitted damage. This can probably be done in one of the following ways:

- There might be some kind of information in the damage packet which indicates the target
- Capture and reverse engineer the client request
