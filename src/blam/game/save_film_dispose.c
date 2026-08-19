/* save_film_dispose @ 0x836EB228 — flush and close any open film */
#include "headers/saved_film.h"
#include "headers/saved_film_mode.h"
extern void ufclose(FILE *stream);
extern void saved_film_flush_buffer(void);
void save_film_dispose(void)
{
    saved_film_flush_buffer();
    if ( global_saved_film.file )
        ufclose(global_saved_film.file);
    global_saved_film.mode = _saved_film_inactive_mode;
    global_saved_film.file = 0;
}
