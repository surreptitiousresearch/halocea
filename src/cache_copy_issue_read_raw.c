/* cache_copy_issue_read_raw @0x83808998 — tail-call forwarder (`li r7, 8; b cache_copy_issue_read_internal`;
 * confirmed via disasm, not a bare blr). The "raw" read variant is just cache_copy_issue_read_internal
 * with a fixed read_buffer_index of 8 (a reserved bypass buffer slot). cache_copy_issue_read_internal
 * itself is not yet decompiled — declared extern with its own DB prototype. */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"

extern void cache_copy_issue_read_internal(simple_decompressor_definition *self, void *buffer, unsigned int size, int offset, int16_t read_buffer_index);

void cache_copy_issue_read_raw(simple_decompressor_definition *self, void *buffer, int size, int offset)
{
    cache_copy_issue_read_internal(self, buffer, size, offset, 8);
}
