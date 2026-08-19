/* saved_film_flush_buffer @ 0x836EAFB0 — write the pending film buffer to disk */

#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"
#include "headers/blam_data_globals.h"

#include <stdio.h>

void saved_film_flush_buffer(void)
{
    if ( global_saved_film.mode == _saved_film_recording_mode
      && saved_film_buffer_tail > 0
      && global_saved_film.file
      && fwrite(saved_film_buffer, saved_film_buffer_tail, 1, global_saved_film.file) == 1 )
    {
        fflush(global_saved_film.file);
    }
    saved_film_buffer_head = 0;
    saved_film_buffer_tail = 0;
}
