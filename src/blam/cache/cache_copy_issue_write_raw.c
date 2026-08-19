/* cache_copy_issue_write_raw @0x838089A0 — tail-call forwarder (`li r7, 1; b cache_copy_issue_write_internal`;
 * confirmed via disasm, not a bare blr). The "raw" write variant is just cache_copy_issue_write_internal
 * with a fixed write_buffer_index of 1. cache_copy_issue_write_internal itself is not yet decompiled —
 * declared extern with its own DB prototype. */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"

extern void cache_copy_issue_write_internal(simple_decompressor_definition *self, void *buffer, int size, int offset, int16_t write_buffer_index);

void cache_copy_issue_write_raw(simple_decompressor_definition *self, void *buffer, int size, int offset)
{
    cache_copy_issue_write_internal(self, buffer, size, offset, 1);
}
