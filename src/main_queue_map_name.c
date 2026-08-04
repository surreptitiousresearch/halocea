/* main_queue_map_name @0x83689330 — set (or clear) the map name queued for the next load. */

#include "headers/main_globals.h"
extern char *strncpy(char *dst, const char *src, unsigned int n);

void main_queue_map_name(const char *new_name)
{
    if ( new_name )
    {
        strncpy(main_globals.queued_map_name, new_name, 0xFFu);
        main_globals.queue_map = 1;
    }
    else
    {
        main_globals.queued_map_name[0] = 0;
        main_globals.queue_map = 0;
    }
}
