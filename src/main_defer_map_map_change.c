/* main_defer_map_map_change @0x83688F40 — clear the pending rename/map-change request, deferring it. */

#include "headers/main_globals.h"

void main_defer_map_map_change(void)
{
    main_globals.rename_map = 0;
}
