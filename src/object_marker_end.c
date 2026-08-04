/* object_marker_end @ 0x836EDC78 — end an object-marking pass. */

#include "headers/object_globals.h"

void object_marker_end(void)
{
    object_globals->object_marker_initialized = 0;
}
