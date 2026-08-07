/* cache_copy_initialize_file_data @0x83808B40 — write a zeroed 2048-byte placeholder cache-file header to
 * the destination (write slot 1 → OVERLAPPED slot 10, in-use bit 0x400), read back the real header from
 * the source (read slot 8, in-use bit 0x100) — each with an alertable 10s wait on the stop event so the
 * completion routines run — verify it, and position both streams past the 2048-byte header block. */

#include <stdint.h>
#include <string.h>
#include "headers/simple_decompressor_definition.h"

extern void cache_copy_issue_write_internal(simple_decompressor_definition *self, void *buffer, int size, int offset, int16_t write_buffer_index);
extern void cache_copy_issue_read_internal(simple_decompressor_definition *self, void *buffer, unsigned int size, int offset, int16_t read_buffer_index);
extern uint8_t cache_file_header_verify(const cache_file_header *header, const char *name, uint8_t fatal);

/* Win32 boundary */
extern unsigned int WaitForSingleObjectEx(void *handle, unsigned int milliseconds, int alertable);

void cache_copy_initialize_file_data(simple_decompressor_definition *self)
{
    cache_file_header *header = &self->header;

    self->async_write_bytes_left = 0;
    memset(&self->header, 0, sizeof(self->header));

    cache_copy_issue_write_internal(self, header, 2048, 0, 1);
    WaitForSingleObjectEx(self->copy_stop_event, 10000u, 1);
    self->overlapped_in_use_flags[0] &= ~0x400u;

    cache_copy_issue_read_internal(self, header, 2048, 0, 8);
    WaitForSingleObjectEx(self->copy_stop_event, 10000u, 1);
    self->overlapped_in_use_flags[0] &= ~0x100u;

    cache_file_header_verify(header, "blah", 1u);

    self->current_request = 0;
    self->current_read_offset = 2048;
    self->current_write_offset = 2048;
    self->current_read_sequence_count = 0;
    self->current_write_request = 0;
    self->read_progress = 0.0f;
    self->read_bytes_left -= 2048;
}
