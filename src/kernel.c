#include "idt.h"
#include "countries.h"
#include "io.h"
#include "pmm.h"
#include "pic.h"
#include "timer.h"
#include "panic.h"

// External References
extern void idt_init();
extern volatile int space_pressed;
extern volatile int exit_game;


// ============================================================
volatile char* video_memory = (volatile char*)0xB8000;

#define VGA_COLS   80
#define VGA_ROWS   25
#define VGA_CELLS  (VGA_COLS * VGA_ROWS)

// Neon Arcade Palette 
#define COL_GREEN     0x0A
#define COL_CYAN      0x0B
#define COL_MAGENTA   0x0D
#define COL_YELLOW    0x0E
#define COL_WHITE     0x0F
#define COL_GREY      0x07

typedef enum { STATE_LANDING = 0, STATE_SPINNING = 1, STATE_RESULT = 2 } GameState;
volatile GameState app_state = STATE_LANDING;

int visited[TOTAL_COUNTRIES] = {0};
int total_spins = 0;
int unique_countries = 0;
int last_idx = -1;


unsigned long int next_rand = 1;
int rand(int max)
{
    next_rand = next_rand * 1103515245 + 12345;
    return (unsigned int)(next_rand / 65536) % max;
}

void delay(unsigned long count)
{
    for (volatile unsigned long i = 0; i < count; i++);
}

int strlen(const char* str)
{
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}



void put_cell(int row, int col, char ch, char color)
{
    if (row < 0 || row >= VGA_ROWS || col < 0 || col >= VGA_COLS) return;
    int pos = (row * VGA_COLS + col) * 2;
    video_memory[pos]     = ch;
    video_memory[pos + 1] = color;
}

// Draws a premium double-line ASCII border
void draw_box(int row1, int col1, int row2, int col2, char color)
{
    for (int c = col1 + 1; c < col2; c++)
    {
        put_cell(row1, c, 0xCD, color); // ═
        put_cell(row2, c, 0xCD, color); // ═
    }
    for (int r = row1 + 1; r < row2; r++)
    {
        put_cell(r, col1, 0xBA, color); // ║
        put_cell(r, col2, 0xBA, color); // ║
    }
    put_cell(row1, col1, 0xC9, color); // ╔
    put_cell(row1, col2, 0xBB, color); // ╗
    put_cell(row2, col1, 0xC8, color); // ╚
    put_cell(row2, col2, 0xBC, color); // ╝
}

// Wipes the screen and automatically draws the global neon edge border
void clear_screen(void)
{
    for (int cell = 0; cell < VGA_CELLS; cell++)
    {
        video_memory[cell * 2]     = ' ';
        video_memory[cell * 2 + 1] = COL_GREY;
    }
    draw_box(0, 0, VGA_ROWS - 1, VGA_COLS - 1, COL_CYAN); // Global Arcade Border
}

void print_at(const char* str, int row, int col, char color)
{
    int pos = (row * VGA_COLS + col) * 2;
    for (int i = 0; str[i] != '\0'; i++)
    {
        video_memory[pos++] = str[i];
        video_memory[pos++] = color;
    }
}

void print_centered(const char* str, int row, char color)
{
    int len = strlen(str);
    int col = (VGA_COLS - len) / 2;
    if (col < 0) col = 0;
    print_at(str, row, col, color);
}

void print_num_at(int num, int row, int col, char color)
{
    if (num == 0)
    {
        print_at("0", row, col, color);
        return;
    }
    char buf[12];
    int i = 0;
    while (num > 0)
    {
        buf[i++] = (num % 10) + '0';
        num /= 10;
    }
    int pos = (row * VGA_COLS + col) * 2;
    while (--i >= 0)
    {
        video_memory[pos++] = buf[i];
        video_memory[pos++] = color;
    }
}

void disable_cursor(void)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20); 
}


void render_landing(void)
{
    clear_screen();
    
    draw_box(3, 20, 7, 59, COL_GREEN);
    print_centered("ENTROPY COMPASS", 5, COL_YELLOW);

    print_centered("A journey through the world", 11, COL_WHITE);
    print_centered("Discover all 187 countries", 13, COL_CYAN);

    draw_box(18, 15, 22, 64, COL_MAGENTA);
    print_centered("[ PRESS SPACE TO SPIN ]", 19, COL_GREEN);
    print_centered("[ HOLD SPACE TO RESET ]", 21, COL_MAGENTA);
}

void render_spin_frame(const char* label)
{
    clear_screen();
    
    draw_box(8, 15, 16, 64, COL_YELLOW);
    print_centered(">>> SPINNING THE GLOBE <<<", 10, COL_YELLOW);
    print_centered(label, 14, COL_CYAN);
}

void render_result(int idx)
{
    clear_screen();
    
    draw_box(2, 20, 4, 59, COL_GREEN);
    print_centered("DESTINATION UNLOCKED", 3, COL_YELLOW);

    print_centered("COUNTRY", 7, COL_GREY);
    print_centered(world_map[idx].name, 8, COL_WHITE);

    print_centered("FOOD", 11, COL_GREY);
    print_centered(world_map[idx].food, 12, COL_YELLOW);

    print_centered("CULTURE", 15, COL_GREY);
    print_centered(world_map[idx].culture, 16, COL_CYAN);

    draw_box(19, 5, 23, 74, COL_MAGENTA);
    print_at("Spins:", 21, 10, COL_GREY);
    print_num_at(total_spins, 21, 17, COL_WHITE);
    
    print_centered("[ PRESS SPACE TO SPIN AGAIN ]", 21, COL_GREEN);
    
    print_at("Found:", 21, 58, COL_GREY);
    print_num_at(unique_countries, 21, 65, COL_CYAN);
}

void perform_spin(void)
{
    app_state = STATE_SPINNING;

    for (int frame = 0; frame < 8; frame++)
    {
        int flash_idx = rand(TOTAL_COUNTRIES);
        render_spin_frame(world_map[flash_idx].name);
        delay(1200000);
    }

    int idx = rand(TOTAL_COUNTRIES);
    total_spins++;
    if (!visited[idx])
    {
        visited[idx] = 1;
        unique_countries++;
    }
    last_idx = idx;

    app_state = STATE_RESULT;
    render_result(idx);
}


void kernel_main(void)
{
    pmm_init(16 * 1024 * 1024);
    idt_init();          
    remap_pic();         
    init_timer(300000);    

    app_state = STATE_LANDING;
    render_landing();
    disable_cursor();

    outb(0x21, 0xFC);    
    outb(0xA1, 0xFF);    
    __asm__ volatile("sti");

    while (1)
    {
        __asm__ volatile("hlt");

        if (exit_game)
        {
            exit_game = 0;
            total_spins = 0;
            unique_countries = 0;
            for (int i = 0; i < TOTAL_COUNTRIES; i++) visited[i] = 0;
            app_state = STATE_LANDING;
            render_landing();
            continue;
        }

        switch (app_state)
        {
            case STATE_LANDING:
            case STATE_RESULT:
                if (space_pressed)
                {
                    space_pressed = 0;
                    perform_spin();
                }
                break;
            case STATE_SPINNING:
                break;
        }
    }
}