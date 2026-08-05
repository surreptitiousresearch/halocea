/* cache_copy_update_write_buffers @0x83808C90 — service the async map-copy worker's single write buffer.
 * Three steps: (1) reap a completed write — if a write is pending/active and the write buffer's overlapped
 * slot (index 9) shows completion, clear the request, its completion bit, and the pending count; (2) if bytes
 * remain and no write is in flight, and the buffered write-request sequence matches the current write sequence
 * (and isn't already the active buffer), issue the next ≤4MB chunk (marking the physical page read-only for the
 * duration) and advance the offset/sequence; (3) if there is no active write buffer and nothing pending, and the
 * write-request slot is free, claim it for the next write sequence and mark its page writable.
 *
 * The decompiler addressed several fields as `self->src_name[bigoffset]`; these are resolved to their real
 * struct members: offset 2460 = overlapped_completed_flags, 2440 = write_buffers[0], 2700 =
 * write_requests[0].write_sequence_index. The buffer/request loops are
 * bounded to the single write buffer, so their counters collapse to index 0. */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"

extern void XPhysicalProtect(void *address, unsigned int size, unsigned int flags);
extern void cache_copy_issue_write_internal(simple_decompressor_definition *self, void *buffer, int size, int offset, int16_t write_buffer_index);

void cache_copy_update_write_buffers(simple_decompressor_definition *self)
{
    /* (1) reap the completed write (single write buffer -> overlapped slot 9). */
    if ( self->write_requests_pending > 0 && self->current_write_request )
    {
        unsigned int write_slot_bit = 1u << 9;
        if ( (write_slot_bit & self->overlapped_completed_flags[0]) != 0 )
        {
            self->write_requests[0].write_sequence_index = -1;
            self->overlapped_completed_flags[0] &= ~write_slot_bit;
            self->current_write_request = nullptr;
            --self->write_requests_pending;
        }
    }

    /* (2) issue the next chunk if one is queued and none is in flight. */
    int write_bytes_left = self->write_bytes_left;
    if ( write_bytes_left )
    {
        if ( !self->current_write_request && self->write_requests_pending > 0 )
        {
            short current_write_buffer_index = self->current_write_buffer_index;
            if ( current_write_buffer_index == -1
              || self->write_requests[0].write_sequence_index > self->current_write_sequence_index )
            {
                if ( current_write_buffer_index != 0
                  && self->write_requests[0].write_sequence_index == self->current_write_sequence_index )
                {
                    self->current_write_request = &self->write_requests[0];
                    int chunk = write_bytes_left < 0x400000 ? write_bytes_left : 0x400000;
                    void *buffer = self->write_buffers[0];
                    XPhysicalProtect(buffer, 0x400000, 2);
                    cache_copy_issue_write_internal(self, buffer, chunk, self->current_write_offset, 0);
                    self->current_write_offset += chunk;
                    self->write_bytes_left -= chunk;
                    ++self->current_write_sequence_index;
                }
            }
        }
    }

    /* (3) claim the free write-request slot for the next sequence when idle. */
    if ( (uint16_t)self->current_write_buffer_index == 0xFFFF && self->write_requests_pending < 1 )
    {
        if ( (uint16_t)self->write_requests[0].write_sequence_index != 0xFFFF )
            return;
        self->write_requests[0].write_sequence_index = self->next_write_sequence_index;
        self->current_write_buffer_index = 0;
        ++self->write_requests_pending;
        ++self->next_write_sequence_index;
        XPhysicalProtect(self->write_buffers[0], 0x400000, 4);
    }
}
