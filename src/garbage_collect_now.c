/* garbage_collect_now @0x836EC890 — request that the object system perform a garbage-collection pass on the
 * next update. */

#include "headers/object_globals.h"
#include "headers/blam_data_globals.h"


void garbage_collect_now(void)
{
    object_globals->force_garbage_collection = 1;
}
