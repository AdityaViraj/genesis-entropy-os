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

## INSTALLATION & BUILD
### 1.Setup environment

**For Linux\\Windows**
```bash
sudo apt update
sudo apt install build-essential nasm qemu-system-x86
```
**For Mac**
```bash
brew install nasm qemu x86_64-elf-gcc
```

### 2.Compile 

**For Linux\\Windows**
```bash
git clone https://github.com/AdityaViraj/genesis-entropy-os
cd genesis-entropy-os
make clean && make
```
**For Mac**
```bash
git clone https://github.com/AdityaViraj/genesis-entropy-os
cd genesis-entropy-os
make clean
make CC=x86_64-elf-gcc LD=x86_64-elf-ld
```

### 3.Launch

**For Linux\\Windows**
```bash
qemu-system-x86_64 -kernel genesis.bin -display gtk,zoom-to-fit=on
```
**For Mac**
```bash
qemu-system-x86_64 -kernel genesis.bin -display cocoa,zoom-to-fit=on
```
