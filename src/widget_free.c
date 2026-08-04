#include "headers/blam_data_globals.h"
#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"
#include "headers/stack_memory_pool.h"
extern void dispose_pointer(stack_memory_pool *pool, void *p);

void widget_free(void *ptr)
{
    dispose_pointer(widget_memory_pool, ptr);
}
