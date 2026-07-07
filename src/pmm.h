// This is our header file used to expose the functionality to the rest of the kernel

#ifndef PMM_H
#define PMM_H
#include <stdint.h>

void pmm_init(uint32_t total_memory);
void test_frame(uint32_t frame_index);
void set_frame(uint32_t frame_index);
void clear_frame(uint32_t frame_index);
int first_free_frame();

#endif

