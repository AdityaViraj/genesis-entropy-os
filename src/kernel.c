/*
Assemly is faster than C because it translates directly to machine code without abstraction , a highly skilled developer can craft assembly instructions that rub faster and use less mermory than compiled C code
Modern compilers are incredibly faster...!!!

To understand how C and Assembly talk to each pther , we have to think as two different levels of the same machine. C provides the high level logic and Assembly provides the raw instructions for the CPU to execute that logic.

The way they communicate is thorugh Calling Convention. When our Assembly code calls kernel_main in C , it is following a specific set of rules agreed upon by the CPU
*/
/*
printf
    ("
    Arguments : Before calling the function Assembly pus the arguments (if any) into specific registers (like rdi,rsi on 64-bit) or pushes them onto the stack
    The call : The Assembly instruction call kernel_main saves the current position of the CPU onto the stack so it knows how to get back later.
    Excution : The C code runs doing its math  logic and memory operations.
    The Return : The C code places its return value in a specific register (usually eax or rax) and executes ret which tells the CPU to jump back to the location it saved on the stack earlier
    ")

printf
        {"
            In C : int x=a+b;
            In assembly : 
            mov eax, [a] ; Move value a into the EAX register
            add eax, [b] ; Add value b to EAX 
            mov [x], eax ;
        
            By writing kernel_main in C we get to use function , varibales and if statements. By callling it from boot.s , we provide the hardware context - the stack , the memory layout and the CPU state - that requires C to run

        "}
*/

// First piece of kernel code. It will clear the screen and print a charecter.
// void kernel_main(void)
// {
//     // The VGA text buffer starts at this memory address
//     char *video_memory = (char *) 0xB8000;

//     // Clear the screen by writing spaces (0x20) and a defaukt color (0x07)
//     // The screen is 80x25 charecters = 2000 cells
//     for(int i=0;i<2000;i++)
//     {
//         video_memory[i*2]=' '; // The charecter
//         video_memory[i*2+1]=0x07; // Attribute byte (Gray on Black)
//     }

//     // Write a message to the top-left corner
//     video_memory[0]='H' ;
//     video_memory[1]=0x0F; // White on Black
// }

/*
        0xB8000 : This is the physcial address in the computers memory space that the hardware routes directly to the monitors display controller.
        i*2 : Since each cell takes 2 bytes ( one for char and one for color ) we skip by 2 for every charecter.
        0x0F : This is the hex value for a white forgeground and black background
*/


/*
    " 
    Environment Engineering: We moved from basic development to 'Bare Metal'. We installed a cross com[iler ( x86_64-elf-gcc ) which is the industry standard for creating software that runs withput an OS 
    Achitecture ( Assembly ) : We wrote boot.s which provides the essential setup . It defines the Multiboot header so a bootloader can find our kernel and it intializes a stack without which a C program would immediately crash.
    Memory Mapping ( Linker Script ) : We built linker.ld to tell the computer exactly where our kernel belongs in physical RAM ( starting at 1MB ) and how to organize the code , data and stack sections in memory
    Hardware intraction ( Kernel C ) : We wrote kernel.c which interacts directly with the physical memory address 0xB8000 - the VGA buffer - to manipulate the screen pixels directly.
    Build Automation : We constructed a Makefile that chains the assembler | compiler and linker together  to produce a single bootable binary file

    How AI teaches us ?
    It processes a vast amounts of technical documentation OSDev Community knowledge and systems engineering standards. Then it applies these specific environment to guide us through the gotchas

    1) THE ENVIRONMENT :
    Bare Metal : Writing code that runs directly on the CPU withpur an operating system underneath it. There are no standard librarues - no printf | no malloc annd no file system
    Cross Compiler : A comiler running on one systems that generates machine code for a completely different environment (a generic , bare metal x86_64 processor). Our Max default compiler builds Mach-O binaries for macOS  we need ELF (Executable and Linked Format) binaries for custom OS kernel
    Linker (ld) : The tool which takes our compiled code fragments (object files) and stitches them into single binary calculating exactly where every variable and function will live in Physical RAM
    
    2) THE BOOTLOADER & ARCHITECTURE
    * Job of a bootloader is to wake up the hardware and load the operating system  into the computers memory..!

    A bootloader is primarliy written in combination of both Assembly labguage and C 
    * Assembly ( The first stage ) : The absolute earliest code executed by the CPU must be written in Assembly ( x86 or ARM assembly ) This is required to intialize core CPU registers , configure basic memory (RAM) , set up the stack pointer and establish a bare minimum environment. On older BIOS systems , this stage had to fit entirely within a tiny 512-byte disk sector
    * C language ( The second stage ) : Once Assembly sets up the basic execution environment , control is passsed to code written in C. C is preferred for the bulk of the bootloader logic becuase it allows developers to write complex code like reading filesystems , handling network configurations and display user menus without dealing with the tediousness of raw assembly.
    When computer powers on , the motherboards firmware looks for bootloader (like GRUB )
    * GRUB - GRand Unified Bootloader, It is the first program that runs whe. we power in a computer and is responsible for loading the OS kernel and presenting a menu to select which OS or kernel version we want to boot

    BIOS - Stands for Basic Input/Output System. It is built in firmware stored that runs. Its primary jobs include :
    * POST (Power On Self Test) : Checking if hardware like RAM,keyboard and hard drivers are working.
    * Hardware Initialisation : Activating the systems basic components ans clock cycles
    * Locationg the OS : Finding the bootloader on a storage drive to hand over control of the computer.
    
    Why 512-Byte limit Existed ?
    The 512-byte restriction is a legacy hardware from the late 1970s and 1980s
    * Sector Size : Early IBM PC hard drives and floppy disks were physically divided into data chunks called sectors. The standard size of one sector was exactly 512 bytes.
    * The MBR rule : The BIOS was hardcoded to look only at the very first sector of the disck known as the Master Boot Record (MBR)
    * Hardware limitations : Early cmputer memory (RAM) and processing power were incredibly small. The BIOS lacked the complexity to read multiple sectors or understand filesystems (like NFTS or FAT32) on its own.
    * The Solution : Programmers wrote a tiny 'Stage 1' bootloader in Assembly that can fit i to thise 512 bytes. Its only job was to locate and load a much larger 'Stage 2' bootloader from elsewhere on the disk.
    Todays modern computers use UEFI (Unified Extensible Firmware Interface) instead of BIOS. UEFI does way with the 512-byte limit and can read large files directly from a standard disk partition.
    
    In the 1970s  AT&T Bell labs invented Unix Operating Systems.
    GNU is a Unix like Operating System developed entirely as free software. The name is a recursive acroynm that stands for 'GNUs Not UNIX!' 
    * Richard Stallmann in 1983 aimed to create a Operating System that gave users total freedom to modify and share the source code.
    * GNU team build almost all the components of an OS (like compiler,text editors anf file utilies ) except the kernel in 1991 Linus Trovalds created Linux Kernel. Developers combined the GNU system software with the Linux Kernel to create a fully functional , free operating system.
    
    * Multiboot Specification : A standard structure at the very top of our binary.It contains a Magic Number (0x1BADB002) and a shecksum. This is a handshake. BRUB scans our files , sees this number and knows ' This is a valid OS I can load it '
    * The Stack Setup (mov esp , stack_top) : CPUs need a temperoru workspace in memory called the stack to keep track of function calls and local variables. Assembly must manually alllcate this memory (resb 16384 for 16KB) and point the CPUs Stack Pointer register (esp) to it before jumping into C code.
    
    3) Memory Mapping (linker.ld)
    The CPU needs to know the physical layout of our operating system in RAM. The linker script dictates the map.
    * . = 1M : Set the current memory address to 1 Megabyte. This is the physical location in RAM where bootloaders safety drop standard kernels.
    * .text : The section where our actual executable machine code lives.
    * .rodata : Read only data (like hardcoded strings)
    * .data : Intialized global variables
    * .bss : Unintialized global variables (memory is reserved but starting at zero)
    
    4) Hardware Interaction
    Because we have no operating system. We communicate with hardware via Memory Mapped I/O.
    * VGA Text Buffer (0xB8000) : The graphics card is hardwired to listein to specific chunk of RAM starting at physical address 0xB8000
    * The 16-bit cell : Every character on the screen requires two bytes of memory. The first byte is the ASCII character. The second byte us the color attribute (eg: 0x0F for white text on a black background). Writing to this memory array instantly changes the pixles on the monitor
    
    5) Build Automation (Makefile)
    * -ffreestanding : Instructs the C compiler not to expect a standard C library
    * -nostdlib : Tells the linker not to link the host OS startup files or libraries
    * The pipeline : It converts Assembly to machine code (.o) compiles C to machine code (.o) and then strcitly follows the rules of linker.ld to fuse them into genesis.bin

*/
#include "idt.h" // This connects kernel.c to out idt_init declaration
#include "countries.h"
#include "io.h"

// External References
extern void idt_init();
extern volatile int space_pressed;
extern volatile int exit_game;

// VGA Memory and Cursor
volatile char* video_memory = (volatile char*)0xB8000;
int cursor_pos=0;

// Game Stats Tracking
int visited[TOTAL_COUNTRIES]={0};
int total_spins=0;
int unique_countries=0;

volatile int app_state=0; // 0: Landing Page, 1: Spinning, 2: Result

// UTILILTY FUNCTIONS

// LCG Random Generator
unsigned long int  next_rand=1;
int rand(int max)
{
    next_rand = next_rand * 1103515245 + 12345;
    return (unsigned int)(next_rand/65536)%max;
}

void clear_screen(void)
{
    for (int i=0;i<80*25*2;i+=2)
    {
        video_memory[i]=' '; // Clear character
        video_memory[i+1]=0x07; //Default color (Gray on Black)
    }
}

int strlen(const char* str)
{
    int len=0;
    while(str[len]!='\0')
    {
        len++;
    }
    return len;
}

void print_centered(const char* str, int row, char color) {
    int len = strlen(str);
    int col = (80 - len) / 2; // Calculate start column for 80-char width
    int cursor_pos = (row * 80 + col) * 2;
    for (int i = 0; str[i] != '\0'; i++) {
        video_memory[cursor_pos++] = str[i];
        video_memory[cursor_pos++] = color;
    }
}

void print_at(const char* str,int row,int col,char color)
{
    cursor_pos=(row*80+col)*2;
    for(int i=0;str[i]!='\0';i++)
    {
        video_memory[cursor_pos++]=str[i];
        video_memory[cursor_pos++]=color; 
    }
}

void print_num_at(int num,int row,int col,char color)
{
    if(num==0)
    {
        print_at("0",row,col,color);
        return;
    }

    char buf[10];
    int i=0;
    while(num>0)
    {
        buf[i++]=(num%10)+'0';
        num/=10;
    }
    cursor_pos=(row*80+col)*2;
    while(--i>=0)
    {
        video_memory[cursor_pos++]=buf[i];
        video_memory[cursor_pos++]=color;
    }
}

void show_landing_page() {
    clear_screen();
    print_centered("****************************************", 5, 0x07);
    print_centered("ENTROPY COMPASS", 7, 0x0E); // Yellow title
    print_centered("****************************************", 9, 0x07);
    
    print_centered("A journey through the world.", 13, 0x0F);
    print_centered("Spin to unlock food and culture.", 14, 0x0F);
    print_centered("Discover all 150+ countries.", 15, 0x0F);
    
    print_centered("[ PRESS SPACE TO SPIN ]", 20, 0x0A);
    print_centered("[ HOLD SPACE TO RESET ]", 22, 0x0C);
}

void disable_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20); // Bit 5 disables the cursor
}


void kernel_main(void)
{
    idt_init(); // Intialize the interrupt system
    show_landing_page();
    disable_cursor(); // Hide the blinking cursor for a cleaner UI
    app_state=0; // Set state to landing page

    while(1)
    {
        // Check for long press exit
        if(exit_game)
        {
            exit_game=0;
            total_spins=0;
            unique_countries=0;
            for(int i=0;i<TOTAL_COUNTRIES;i++)
            visited[i]=0;
            show_landing_page();
        }

        // Spinlogic
        if(space_pressed)
        {
            space_pressed=0;
            app_state=1; // Set state to spinning

            clear_screen();
            print_centered(">>> SPINNING THE GLOBE..!! <<<",12,0x0E); //Bright Yellow
            
            //Delay
            for(volatile unsigned long i=0;i<10000000;i++);
            
            //Math
            int idx=rand(TOTAL_COUNTRIES);
            total_spins++;
            if(visited[idx]==0)
            {
                visited[idx]=1;
                unique_countries++;
            }

            // Results UI
            clear_screen();
            print_centered("=== DESTINATION UNLOCKED ===", 4, 0x0A);
            
            print_centered("COUNTRY", 7, 0x07);
            print_centered(world_map[idx].name, 8, 0x0F);
            
            print_centered("FOOD", 11, 0x07);
            print_centered(world_map[idx].food, 12, 0x0E);
            
            print_centered("CULTURE", 15, 0x07);
            print_centered(world_map[idx].culture, 16, 0x0B);
            
            print_centered("------------------------", 19, 0x08);
            
            print_at("Spins:", 21, 33, 0x07);
            print_num_at(total_spins, 21, 40, 0x0F);
            
            print_at("Found:", 22, 33, 0x07);
            print_num_at(unique_countries, 22, 40, 0x0A);
            
            print_centered("[ PRESS SPACE TO SPIN AGAIN ]", 24, 0x08);
        }
    }
}

// void spin_globe()
// {
//     for(int i=0;i<20;i++)
//     {
//         int random_index=rand(4); 
//         clear_screen();
//         terminal_print(">>> SPINNING GLOBE... <<<");
//         terminal_print(world_map[random_index].name);

//         // Simple delay loop
//         for(volatile int j=0;j<5000000;j++);
//     }
// }


/*
    * Interrupt Driven Input : Our keyboard_handler_c is the "trigger". Because it is called by an Interrupt Service Routine (ISR) the game responds instantly to our input regardless of what the CPU is doing.
    * Memory Efficiency : By storing the country database in a struct array  we keep the data in the kernel's binary eliminating the need for a file system or external memory management of the MVP
    * VGA Text Mode : Printing directly to 0xB8000 is the fastest way to display information on bare metal ensuring our spinning globe animation is smoooth and responsive.
*/

// 

/*
    const char* str : This allows us to pass any string (like "Hello World") to the function.
    static int cursor : By maing this static  the variable "remembers" its value between calls. Every time we print a new charecter it advances to the next availible screen position.
    str[i]!='\0' : In C all strings end with a "null terminator". This loops runs until it hits the invisible end of string marker.
*/

/*
    C17 Language Standard : This project uses the C17 language standard version. It is a revision of the C11 standard and includes bug fixes and clarifications. It does not introduce new features but ensures better compatibility and stability for existing C code.
    The C17 standard is widely supported by modern compilers including GCC and Clang making it a reliable choice for developing low-level systems like operating systems.
    By adhering to the C17 standard we can ensure that our kernel code is portable and maintainable across different platforms and compiler versions.
*/