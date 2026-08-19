/* save_film_end_playback @ 0x836EBB80 — close a film being played back */

#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"

extern void saved_film_flush_buffer(void);
extern void ufclose(FILE *stream);

void save_film_end_playback(void) /* attested void: tail-call of void fn, 0/2 callers consume r3 */
{
    if ( global_saved_film.mode == _saved_film_playback_mode )
    {
        if ( global_saved_film.file )
            ufclose(global_saved_film.file);
        global_saved_film.file = 0;
        global_saved_film.mode = _saved_film_inactive_mode;
    }
    saved_film_flush_buffer();
}
