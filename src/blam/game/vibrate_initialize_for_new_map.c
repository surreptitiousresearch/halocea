/* vibrate_initialize_for_new_map @0x83739158 — zero the controller-vibration globals. */

#include "headers/vibrate_global_data.h"
#include <string.h>
#include "headers/blam_data_globals.h"


void vibrate_initialize_for_new_map(void)
{
    /* attested void: tail-branch into memset; sole caller ignores r3 */
    memset(vibrate_globals, 0, sizeof(vibrate_global_data));
}
