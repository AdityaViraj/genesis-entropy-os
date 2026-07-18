# Entropy Compass

This project is a custom baremetal x86 operating system which is entirely built from scratch in C doubling as a travel retro arcade game which entirely works on **ONEKEY**. The globe starts spinning the moment you hit the key and it randomly selects a country and you can repeat this task until you hit all the 187 countries in the least number of spins.
Well it took me 1047 spins (my best score so far..! Maybe I will include a scoreboard during future iterations!).

* **Tap SPACE:** To start spinning
* **Hold SPACE:** To restart the game

---

## DEMO 
<img width="800" height="519" alt="globe-spinning" src="https://github.com/user-attachments/assets/c7c714ad-6ddb-4df0-8a51-9e04c2593b47" />

---

## HOW DOES THIS WORK ?

Building it from scratch means we don't have access to any standard libararies.
So we starting off with the Bootloader which takes control as soon as we launch the OS this sets up the intial pointer and transtions the CPU into 32 bit protected mode before handing its execution over to the C kernel.

To manage the memory I have built a custom Physical Memory Manager which ses a bitmap arrays where every single bit represents each 4KB frame of physical memory.So when the OS requires RAM it just scans the bitmap to find the O and flips it to 1 and then returns the memory address. This ensures that the allocations are made faster!

And when you press the space bar how does the game know? So to handle this I had to program the Programmable Interrupt Controller so when you press the key , the keyboard sends an electrical signal to the CPU this makes the CPU halt its current tasks and look up into the interrupt description table and then jumps to the asssembly wrapper to safely save the CPU register and it then executes the C keyboard driver which leads to the restoration of the CPU state and resumes the game! 

And how we choose a random destination as we don't have rand() function? We use Programmable Interval Timer hardware as the main source of the entropy. When we tap the key the OS intercepts which uses this unpredictable rapidly changing hardware counter to mathematically calculate a totally random index and push it as the winning destination to the VGA buffer.

---

## Requirements

You need these three tools, regardless of OS:

* **`nasm`** 
* **A 32-bit-capable freestanding C compiler** — a real `x86_64-elf-gcc` cross compiler on macOS, or plain `gcc` (with 32-bit support) on Linux
* **`qemu-system-x86_64`** 

The section below give copy and pasteable setup commands for macOS, Linux, and Windows. Pick the one that matches your machine, run the commands top to bottom in your terminal, and you are done...!!

---

## Built and Run

### macOS

Requires [Homebrew](https://brew.sh). If you don't have it yet, the first command installs it.

```bash
# 1. Install Homebrew if you don't already have it on your computer.
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 2. Install the build tools: assembler, emulator, and ELF cross compiler
brew install nasm qemu x86_64-elf-gcc x86_64-elf-binutils

# 3. Clone the repo 
git clone https://github.com/AdityaViraj/genesis-entropy-os
cd genesis-entropy-os

# 4. Build and run
make clean && make
qemu-system-x86_64 -kernel genesis.bin -display cocoa,zoom-to-fit=on
```

### Linux (Debian/Ubuntu and derivatives)

```bash
# 1. Install the build tools
sudo apt update
sudo apt install -y build-essential gcc-multilib nasm qemu-system-x86 git

# 2. Clone the repo
git clone https://github.com/AdityaViraj/genesis-entropy-os
cd genesis-entropy-os

# 3. Build and run
make clean && make
qemu-system-x86_64 -kernel genesis.bin -display gtk,zoom-to-fit=on
```

Using Fedora/RHEL/Arch instead of apt? Swap step 1 for your package manager's equivalent of `gcc`, `nasm`, `qemu-system-x86`, and `git` (e.g. `sudo dnf install gcc nasm qemu-system-x86 git` or `sudo pacman -S gcc nasm qemu git`), then continue from step 2.

### Windows

Native Windows doesn't have the tooling this project needs, so the best recommended path is **WSL2 (Windows Subsystem for Linux)**, which gives you a real Ubuntu environment inside the Windows.

```powershell
# 1. In PowerShell (as Administrator), install WSL2 with Ubuntu
wsl --install -d Ubuntu
```

Restart your computer if prompted, then open the **Ubuntu** app from your Start Menu, finish the one-time Linux user setup, and run the rest of these commands **inside that Ubuntu/WSL terminal**:

```bash
# 2. Install the build tools
sudo apt update
sudo apt install -y build-essential gcc-multilib nasm qemu-system-x86 git

# 3. Clone the repo
git clone https://github.com/AdityaViraj/genesis-entropy-os
cd genesis-entropy-os

# 4. Build and run
make clean && make
qemu-system-x86_64 -kernel genesis.bin -display gtk,zoom-to-fit=on
```

The QEMU window opens automatically via WSLg (built into Windows 11, and available on Windows 10 21H2+ after `wsl --update`). If no window appears, run `wsl --update` in PowerShell, then try again.

---

### One-liner (any OS, after dependencies are installed)

Once `nasm`, a working `gcc`/`x86_64-elf-gcc`, and `qemu-system-x86_64` are on your PATH, you can always just run:

```bash
make clean && make run
```

The Makefile detects your OS automatically and picks the right compiler and QEMU display flags for you.

---

## Troubleshooting

* **`make: x86_64-elf-gcc: No such file or directory` (macOS):** Homebrew's cross compiler isn't installed or isn't on your PATH — rerun `brew install x86_64-elf-gcc x86_64-elf-binutils`, then `brew doctor`.
* **`fatal error: stdint.h: No such file or directory` (Linux):** `gcc-multilib` didn't install correctly — rerun `sudo apt install -y gcc-multilib`.
* **QEMU opens but shows a black screen:** give it a second — text-mode VGA output appears once the kernel finishes booting and initializing interrupts.
* **No window appears at all on WSL:** run `wsl --update` in PowerShell (outside WSL), then restart the Ubuntu terminal and try again.
* **`Could not initialize SDL/GTK display` (Linux):** your QEMU build lacks the GTK UI. Drop `,zoom-to-fit=on` and use plain `-display sdl` instead, or reinstall with `sudo apt install --reinstall qemu-system-gui` (Ubuntu) to pull in the GTK-enabled build.
