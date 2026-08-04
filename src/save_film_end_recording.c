/* save_film_end_recording @ 0x836EB680 — flush and close an in-progress recording */

#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"

extern void saved_film_flush_buffer(void);
extern void ufclose(FILE *stream);

void save_film_end_recording(void)
{
    if ( global_saved_film.mode == _saved_film_recording_mode )
    {
        saved_film_flush_buffer();
        if ( global_saved_film.file )
            ufclose(global_saved_film.file);
        global_saved_film.file = 0;
        global_saved_film.mode = _saved_film_inactive_mode;
    }
}
