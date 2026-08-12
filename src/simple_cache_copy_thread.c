/* simple_cache_copy_thread @0x83809080 — the worker-thread entry point for the async "simple cache" map
 * copy/decompress path. It loops forever: each pass waits on copy_start_event, primes the read state, and (unless
 * a stop was already signalled) verifies the compressed cache header, initialises zlib, issues the initial batch
 * of 8 read requests (marking their overlapped slots in use), then runs the copy/decompress pump until the source
 * is exhausted or a stop event fires. The pump acquires read requests, waits (alertably) on the stop event while
 * I/O completes, and on each 10s "proceed" tick updates the write buffers and runs a decompression step; a
 * timeout sets the abort flag. When all bytes are written it flushes the header, tears down zlib, waits for
 * outstanding I/O, closes the read file, and signals copy_complete_event before looping back.
 *
 * DEVIATION: the decompiler lost the overlapped_in_use_flags field and rendered the read-request bitmask write as
 * `*(_DWORD*)&self->src_name[4*((v5>>5)+614)]` — 4*614 = 0x998 is exactly &overlapped_in_use_flags, and v5<8 so
 * v5>>5 is always 0; restored as `overlapped_in_use_flags |= 1<<v5`. The one-iteration `for(i=0;i<1)` buffer-
 * in-use scan is reproduced as a single scalar test. The `192`/`258` wait results are the function's own
 * "proceed" sentinel and WAIT_TIMEOUT. */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include "headers/cache_file_header.h"
#include "headers/cache_copy_read_request.h"
#include "headers/z_stream.h"
#include "headers/copy_flags.h"

#include "headers/cache_copy_read_request.h"
#include "headers/cache_copy_read_request.h"
extern void cache_copy_initialize_and_fill_with_garbage(simple_decompressor_definition *self);
extern void cache_copy_initialize_read_data(simple_decompressor_definition *self);
extern void cache_copy_initialize_file_data(simple_decompressor_definition *self);
extern uint8_t cache_file_header_verify(const cache_file_header *header, const char *name, uint8_t fatal);
extern void cache_copy_issue_read_request_internal(simple_decompressor_definition *self, cache_copy_read_request *request, int16_t read_buffer_index);
extern cache_copy_read_request * acquire_read_request(simple_decompressor_definition *self, int16_t read_sequence_index);
extern void cache_copy_update_write_buffers(simple_decompressor_definition *self);
extern void cache_copy_run_decompression(simple_decompressor_definition *self);
extern void wait_for_io_to_complete(simple_decompressor_definition *self);
extern void cache_copy_issue_write_internal(simple_decompressor_definition *self, void *buffer, int size, int offset, int16_t write_buffer_index);
extern int inflateInit_(z_stream *strm, const char *version, int stream_size);
extern int inflateEnd(z_stream *strm);

extern unsigned int WaitForSingleObject(void *handle, unsigned int milliseconds);
extern unsigned int WaitForSingleObjectEx(void *handle, unsigned int milliseconds, int alertable);
extern int SetEvent(void *hEvent);
extern int CloseHandle(void *hObject);

/* Win32 thread entry point: receives lpParameter (r3) per the LPTHREAD_START_ROUTINE contract but ignores
 * it (state lives in global_self). The param is declared so the extern matches the CreateThread call site
 * in cache_copy_initialize.c; disasm never reads r3, hence unused. 2026-07-31 */
unsigned int simple_cache_copy_thread(void *parameter)
{
    (void)parameter;
    simple_decompressor_definition *self = global_self;

    while ( 1 )
    {
        WaitForSingleObject(self->copy_start_event, 0xFFFFFFFF);
        cache_copy_initialize_and_fill_with_garbage(self);
        cache_copy_initialize_read_data(self);

        if ( WaitForSingleObject(global_self->copy_stop_event, 0) )
        {
            cache_copy_initialize_file_data(self);
            self->zlib_stream.next_in = nullptr;
            self->zlib_stream.avail_in = 0;
            self->zlib_stream.next_out = nullptr;
            self->zlib_stream.avail_out = 0;
            inflateInit_(&self->zlib_stream, "1.1.3", 56);

            if ( cache_file_header_verify(&self->header, "cache decompressed", 1) )
            {
                bool active = 1;
                int16_t read_sequence = 0;
                int bytes_to_write = self->header.size - 2048;
                self->async_write_bytes_left = bytes_to_write;
                self->write_bytes_left = bytes_to_write;

                int read_index = 0;
                do
                {
                    cache_copy_issue_read_request_internal(self, &self->read_requests[read_index], read_sequence);
                    read_sequence = (int16_t)(read_index + 1);
                    /* decompiler rendered this as src_name[4*((read_index>>5)+614)]; 4*614 == &overlapped_in_use_flags */
                    self->overlapped_in_use_flags[0] |= 1u << (read_index & 0x1F);
                    read_index = read_sequence;
                }
                while ( read_sequence < 8 );

                if ( WaitForSingleObject(global_self->copy_stop_event, 0) )
                {
                    do
                    {
                        if ( self->write_bytes_left <= 0 || !active )
                            break;

                        char buffer_in_flight = 0;   /* decompiler: one-iteration for(i=0;i<1) over overlapped_in_use_flags[i] */
                        if ( self->overlapped_in_use_flags[0] )
                            buffer_in_flight = 1;

                        unsigned int wait_result;
                        if ( !buffer_in_flight )
                        {
                            wait_result = 192;
                        }
                        else
                        {
                            bool no_request = acquire_read_request(self, self->current_sequence_index) == nullptr;
                            bool write_ready = 0;
                            if ( self->write_requests_pending == 1
                              && (uint16_t)self->current_write_buffer_index == 0xFFFF )
                            {
                                write_ready = 1;
                            }

                            if ( no_request || write_ready )
                            {
                                SetEvent(self->progress_update_event);
                                wait_result = WaitForSingleObjectEx(self->copy_stop_event, 0x2710, 1);
                            }
                            else
                            {
                                wait_result = 192;
                            }
                        }

                        active = 0;
                        if ( wait_result == 192 )
                        {
                            cache_copy_update_write_buffers(self);
                            cache_copy_run_decompression(self);
                            active = (global_self->flags & 7) == 0;
                        }
                        else if ( wait_result == 258 )
                        {
                            global_self->flags |= (1u << _copy_read_failed_bit);
                        }
                    }
                    while ( WaitForSingleObject(global_self->copy_stop_event, 0) );
                }

                if ( !self->write_bytes_left )
                {
                    wait_for_io_to_complete(self);
                    cache_copy_issue_write_internal(self, &self->header, 2048, 0, 1);
                }
            }

            inflateEnd(&self->zlib_stream);
            self->zlib_stream.next_in = nullptr;
            self->zlib_stream.avail_in = 0;
            self->zlib_stream.next_out = nullptr;
            self->zlib_stream.avail_out = 0;
        }

        wait_for_io_to_complete(self);
        CloseHandle(self->read_file_handle);
        self->read_file_handle = nullptr;
        self->write_file_handle = nullptr;
        SetEvent(self->copy_complete_event);
    }
}
