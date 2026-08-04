/* ui_widget_realloc @0x83730840 — tail-call forwarder (confirmed via disasm/decompiler, not a bare blr).
 * Resizes a widget-pool allocation. */

#include <stdint.h>
#include "headers/stack_memory_pool.h"
#include "headers/blam_data_globals.h"

extern void *pool_resize_pointer(stack_memory_pool *pool, void *p, unsigned int new_size);

void * ui_widget_realloc(void *ptr, uint16_t size)
{
    return pool_resize_pointer(widget_memory_pool, ptr, size);
}
