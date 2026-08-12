/* cache_copy_run_decompression @0x83808EC0 — the decompression worker loop for the async cache-copy path.
 * Each pass: yield (alertable SleepEx so queued ReadFileEx completions run), refill the write-buffer bookkeeping,
 * pull a fresh 128 KiB compressed read buffer into zlib's input when the input is drained, point zlib at the
 * current 4 MiB output buffer when its output is drained, then inflate. On inflate error it flags a stop; when
 * the input block is fully consumed it recycles that read request (re-issues the read, advances the sequence,
 * decrements the in-flight count); when the output fills or the stream ends it releases the write buffer. Exits
 * when there is no read request left, no write buffer available, or on a zlib error.
 *
 * DEVIATION: Hex-Rays rendered the request-slot index as a raw pointer-diff `((char*)req - (char*)self - 2684)
 * >> 1` (2684 = 0xA7C = read_requests[]) and treated the single overlapped_completed_flags dword as an array
 * (`[v8 >> 5]`, always [0] for slots 0-7). Expressed here as request - self->read_requests and a scalar mask. */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include "headers/cache_copy_read_request.h"
#include "headers/z_stream.h"
#include "headers/copy_flags.h"

extern void cache_copy_update_write_buffers(simple_decompressor_definition *self);
extern cache_copy_read_request * acquire_read_request(simple_decompressor_definition *self, int16_t read_sequence_index);
extern void cache_copy_issue_read_request_internal(simple_decompressor_definition *self, cache_copy_read_request *request, int16_t read_buffer_index);
extern int inflate(z_stream *strm, int flush);

extern unsigned int SleepEx(unsigned int milliseconds, int alertable);
extern void SwitchToThread(void);
extern unsigned int WaitForSingleObject(void *handle, unsigned int milliseconds);

void cache_copy_run_decompression(simple_decompressor_definition *self)
{
    z_stream *zlib_stream = &self->zlib_stream;

    while ( 1 )
    {
        SleepEx(0, 1);
        cache_copy_update_write_buffers(self);

        if ( !zlib_stream->avail_in )
        {
            cache_copy_read_request *request = acquire_read_request(self, self->current_sequence_index);
            self->current_request = request;
            if ( !request )
                return;
            zlib_stream->avail_in = 0x20000;
            zlib_stream->next_in = (unsigned char *)self->read_buffers[request - self->read_requests];
            self->current_read_sequence_count = 1;
        }

        if ( self->current_write_buffer_index == -1 )
            break;

        if ( !zlib_stream->avail_out )
        {
            zlib_stream->avail_out = 0x400000;
            zlib_stream->next_out = (unsigned char *)self->write_buffers[self->current_write_buffer_index];
        }

        if ( zlib_stream->avail_in && zlib_stream->avail_out )
        {
            if ( !global_self->blocking )
                SwitchToThread();

            unsigned int inflate_result = inflate(zlib_stream, 0);
            if ( inflate_result > 1 )
            {
                if ( WaitForSingleObject(global_self->copy_stop_event, 0) )
                    global_self->flags |= (1u << _copy_decompression_failed_bit);
                return;
            }

            if ( !zlib_stream->avail_in )
            {
                cache_copy_read_request *current_request = self->current_request;
                int16_t request_index = (int16_t)(current_request - self->read_requests);
                self->overlapped_completed_flags[0] &= ~(1 << (request_index & 0x1F));
                current_request->read_sequence_index = -1;
                cache_copy_issue_read_request_internal(self, current_request, request_index);
                int16_t sequence_index = self->current_sequence_index;
                int16_t read_sequence_count = self->current_read_sequence_count;
                self->current_request = 0;
                self->current_sequence_index = sequence_index + 1;
                self->current_read_sequence_count = read_sequence_count - 1;
            }

            if ( !zlib_stream->avail_out || inflate_result == 1 )
                self->current_write_buffer_index = -1;
        }
    }
}
