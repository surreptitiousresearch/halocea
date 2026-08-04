/* saved_film_read @0x836EB0F4 — read up to *size bytes of the saved film stream into buffer (playback mode
 * only). Serves from the 16 KB ring buffer, refilling from file (compacting any leftover to the front) when
 * it doesn't already hold enough. On return *size is the number of bytes produced. Returns 1 on success,
 * 0 on bad args / wrong mode / end of data. */

#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"
#include "headers/blam_data_globals.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint8_t saved_film_read(void *buffer, int16_t *size)
{
    if ( !buffer )
        return 0;
    if ( !size )
        return 0;

    struct _iobuf *file = global_saved_film.file;
    if ( !global_saved_film.file || global_saved_film.mode != _saved_film_playback_mode )
        return 0;

    int head = saved_film_buffer_head;
    int tail = saved_film_buffer_tail;
    signed int available = saved_film_buffer_tail - saved_film_buffer_head;
    if ( available < *size )
    {
        if ( available > 0 )
        {
            memmove(saved_film_buffer, &saved_film_buffer[saved_film_buffer_head], available);
            file = global_saved_film.file;
            tail = saved_film_buffer_tail;
            head = saved_film_buffer_head;
        }
        saved_film_buffer_tail = tail - head;
        saved_film_buffer_head = 0;
        unsigned int read = fread(&saved_film_buffer[tail - head], 1u,
                                  SAVED_FILM_IO_BUFFER_SIZE - (tail - head), file);
        head = saved_film_buffer_head;
        tail = read + saved_film_buffer_tail;
        saved_film_buffer_tail += read;
    }

    int remaining = tail - head;
    if ( remaining <= 0 )
        return 0;
    signed int to_copy = *size;
    if ( to_copy > remaining )
        to_copy = remaining;
    memcpy(buffer, &saved_film_buffer[head], to_copy);
    *size = to_copy;
    saved_film_buffer_head = head + to_copy;
    return 1;
}
