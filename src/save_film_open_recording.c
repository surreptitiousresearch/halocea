/* save_film_open_recording @0x836EB6F4 — open a saved-film file for playback ("recording" here means
 * the recorded film being replayed): flush any pending buffer, reset the per-frame action state, close
 * any film currently open, then open the file read-binary. On success enter playback mode (2) and reset
 * the expected-update counter. Returns 1 on success, 0 if the file could not be opened. */

#include <stdint.h>
#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"
#include "headers/blam_data_globals.h"

extern void saved_film_flush_buffer(void);
extern void *memset(void *dst, int value, unsigned int count);
extern void ufclose(FILE *stream);
/* fopen provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

uint8_t save_film_open_recording(const char *filename)
{
    saved_film_flush_buffer();
    global_saved_film.player_count = 0;
    memset(global_saved_film.action_updates, 0, sizeof(global_saved_film.action_updates));
    if ( global_saved_film.mode )
    {
        if ( global_saved_film.file )
        {
            ufclose(global_saved_film.file);
            global_saved_film.file = 0;
        }
        global_saved_film.mode = _saved_film_inactive_mode;
    }
    global_saved_film.file = fopen(filename, "rb");
    if ( !global_saved_film.file )
        return 0;
    global_saved_film.mode = _saved_film_playback_mode;
    global_saved_film.next_expected_update = 0;
    return 1;
}
