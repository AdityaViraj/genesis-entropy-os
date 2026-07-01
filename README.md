# Entropy Compass

A minimalist , bare-metal OS built from scratch in C and Assembly. **Entropy Compass** is an experimental "Travel Roulette" platform that challenges users to discover the world through a single key interface.

### How to Run

Ensure you have `qemu` and the `x86_64-elf` cross-compiler installed.

```bash
make clean && make
qemu-system-x86_64 -kernel genesis.bin -display cocoa,zoom-to-fit=on -device usb-tablet

```

### Controls

* **Tap SPACE**: Spin the globe to unlock a random destination.
* **Hold SPACE**: Reset your journey and return to the home screen.
* PRESS CONTROL + OPTION + G to exit

*Built as an MVP for  low level hardware studies.*
