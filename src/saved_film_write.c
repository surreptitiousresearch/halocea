/* saved_film_write @0x836EB044 — append a buffer to the saved-film write buffer while recording, flushing to
 * disk first if it would overflow. Returns 1 on success, 0 on failure (not recording, no room, bad args).
 *
 * Deviation: the decompiler returns an uninitialized stack byte on the post-flush overflow path; that path is
 * a failure, so 0 is returned. */

#include <stdint.h>
#include <string.h>
#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"
#include "headers/blam_data_globals.h"


extern void saved_film_flush_buffer(void);

uint8_t saved_film_write(void *buffer, int16_t size)
{
    if ( !buffer )
        return 0;
    if ( size <= 0 || !global_saved_film.file || global_saved_film.mode != _saved_film_recording_mode )
        return 0;

    int tail = saved_film_buffer_tail;
    if ( SAVED_FILM_IO_BUFFER_SIZE - saved_film_buffer_tail < size )
    {
        saved_film_flush_buffer();
        tail = saved_film_buffer_tail;
    }
    if ( SAVED_FILM_IO_BUFFER_SIZE - tail < size )
        return 0;
    memcpy(&saved_film_buffer[tail], buffer, size);
    saved_film_buffer_tail = size + tail;
    return 1;
}
