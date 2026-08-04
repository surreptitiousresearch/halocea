/* object_marker_begin @ 0x836EDC50 — start an object-marking pass: bump the global object marker so
 * previously-marked objects no longer match, and flag the marker as active. */

#include "headers/object_globals.h"

void object_marker_begin(void)
{
    ++global_object_marker;
    object_globals->object_marker_initialized = 1;
}
