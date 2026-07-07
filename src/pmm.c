// In this code we use bitwise operations to manage the frames.

#include "pmm.h"
#include "io.h"

// EACH BIT REPRESENTS A ONE FRAME OF 4KB
#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096

static uint32_t *memory_bitmap;
static uint32_t total_frames;
extern uint32_t _kernel_end;


void pmm_init(uint32_t mem_size_bytes)
{
    total_frames=mem_size_bytes/4096;

    memory_bitmap=(uint32_t*)&_kernel_end;

    for(uint32_t i=0;i<(total_frames/32);i++)
    {
        memory_bitmap[i]=0xFFFFFFFF;
    }

}

void set_frame(uint32_t frame_index)
{
    memory_bitmap[frame_index/32] |= (1<<(frame_index%32));
}

int allocate_frame()
{
    for(uint32_t i=0;i<(total_frames/32);i++)
    {
        if(memory_bitmap[i]!=0xFFFFFFFF)
        {
            for(int j=0;j<32;j++)
            {
                if(!(memory_bitmap[i] & (1<<j)))
                {
                    set_frame(i*32+j);
                    return (i*32+j);
                }
            }
        }
    }
    return -1;
}

