/* main_disallow_persistent_storage @0x83688E30 — clear the flag that lets the next solo map load from
 * persistent storage. */

#include "headers/main_globals.h"

void main_disallow_persistent_storage(void)
{
    main_globals.solo_try_and_load_from_persistent_storage = 0;
}
