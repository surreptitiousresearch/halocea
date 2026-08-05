/* object_name_list_allocate @0x836EE028 — allocate the 512-entry object-name lookup table.
 *
 * A flat int[512] mapping scenario object-name index -> object index (2048 bytes), allocated from the
 * game-state arena and stashed in the object_name_list global. */

#include "headers/blam_data_globals.h"

extern void *game_state_malloc(const char *name, const char *type, int size);

void object_name_list_allocate(void)
{
    object_name_list = game_state_malloc("object name list", 0, 2048);
}
