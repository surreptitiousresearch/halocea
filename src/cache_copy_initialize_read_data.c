/* cache_copy_initialize_read_data @0x83808178 — open the copy worker's source file for buffered async
 * reading (FILE_FLAG_NO_BUFFERING|FILE_FLAG_OVERLAPPED), size all the byte counters from it, clear the
 * OVERLAPPED slots and in-use/completed masks, reset the 8 read-request and 1 write-request sequence
 * indices to NONE, and zero the sequence counters. */

#include <stdint.h>
#include <string.h>
#include "headers/simple_decompressor_definition.h"

/* Win32 boundary */
extern void *CreateFileA(const char *file_name, unsigned int desired_access, unsigned int share_mode,
        void *security_attributes, unsigned int creation_disposition, unsigned int flags_and_attributes,
        void *template_file);
extern unsigned int GetFileSize(void *file, unsigned int *file_size_high);

void cache_copy_initialize_read_data(simple_decompressor_definition *self)
{
    void *read_file = CreateFileA(self->src_name, 0x80000000, 0, 0, 3u, 0x60000000u, 0);
    self->read_file_handle = read_file;

    int file_size = GetFileSize(read_file, 0);
    self->read_bytes_left = file_size;
    self->read_file_size = file_size;
    self->async_read_bytes_left = file_size;

    memset(self->overlapped, 0, sizeof(self->overlapped));
    self->overlapped_in_use_flags[0] = 0;
    self->overlapped_completed_flags[0] = 0;

    for ( int request_index = 0; request_index < 8; request_index = (int16_t)(request_index + 1) )
        self->read_requests[request_index].read_sequence_index = -1;
    for ( int request_index = 0; request_index < 1; request_index = (int16_t)(request_index + 1) )
        self->write_requests[request_index].write_sequence_index = -1;

    self->current_write_buffer_index = -1;
    self->current_read_sequence_index = 0;
    self->current_sequence_index = 0;
    self->current_write_sequence_index = 0;
    self->next_write_sequence_index = 0;
    self->write_requests_pending = 0;
}
