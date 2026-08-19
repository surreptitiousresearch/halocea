/* main_movie_stop @0x8368AAE0 — end movie capture and free the capture bitmap. */

#include "headers/main_globals.h"

extern void bitmap_delete(bitmap_data *bitmap);

void main_movie_stop(void)
{
    if ( main_globals.movie )
    {
        bitmap_delete(main_globals.movie);
        main_globals.movie = 0;
    }
}
