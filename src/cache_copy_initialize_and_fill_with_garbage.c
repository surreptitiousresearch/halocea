/* cache_copy_initialize_and_fill_with_garbage @0x838080B0 — carve the copy worker's allocated buffer into
 * its 8 read buffers (128KB each) and 1 write buffer (4MB), write-combine-protect the 5MB block
 * (XPhysicalProtect flag 2), point the zlib scratch buffer (72KB budget) and allocation cursor at the end
 * of that block, and poison the trailing runtime state (from read_file_handle through the end of the
 * struct, 0x134 bytes) with 0xFA garbage.
 *
 * DEVIATION: the decompiler renders the buffer-array stores as indexed writes off global_self->src_name;
 * they are self's read_buffers[]/write_buffers[] slots (dword offsets 602/610 == 0x968/0x988) — written
 * through global_self exactly as compiled (global_self aliases `self` here). */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include <string.h>
#include "headers/blam_data_globals.h"
#include "headers/cache_copy_buffer_sizes.h"


extern void XPhysicalProtect(void *address, unsigned int size, unsigned int flags); /* Xbox kernel boundary */

void cache_copy_initialize_and_fill_with_garbage(simple_decompressor_definition *self)
{
    char *cursor = (char *)self->allocated_buffer;

    for ( int16_t i = 0; i < 8; i++ )
    {
        global_self->read_buffers[i] = cursor;
        cursor += 0x20000;
    }
    for ( int16_t i = 0; i < 1; i++ )
    {
        global_self->write_buffers[i] = cursor;
        cursor += 0x400000;
    }

    XPhysicalProtect(self->allocated_buffer, TOTAL_READ_WRITE_BUFFER_SIZE, 2u);

    self->zlib_buffer = (unsigned char *)self->allocated_buffer + 5242880;
    self->zlib_buffer_size = 73728;
    self->next_allocation = (unsigned char *)self->allocated_buffer + 5242880;
    memset(&self->read_file_handle, 250, 0x134u);
}
