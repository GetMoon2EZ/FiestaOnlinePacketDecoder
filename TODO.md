# TODO

- [ ] Implement Unknown packet (generic solution)
- [ ] Find a way to distinguish damage done and damage received
- [ ] All time damage ?
- [ ] Add unit tests
- [ ] Customize default GUI
- [ ] Make IMGUI a submodule

# Precisions

As of right now, the DPS is calculated using every damage packet that is received by the client. Some of these packets represent damage done by an enemy to the player. Those packets should somehow be distinguished from the player's emitted damage. This can probably be done in one of the following ways:
* There might be some kind of information in the damage packet which indicates the target
* Capture and reverse engineer the client request
