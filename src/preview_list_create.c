/* preview_list_create @0x837806B0 — one-time init of the 3 preview list banks (16-byte elements). */

#include "headers/dynamic_array.h"
#include "headers/blam_data_globals.h"


void preview_list_create(void)
{
    dynamic_array_new(&preview_list_array_bank[0], 16);
    dynamic_array_new(&preview_list_array_bank[1], 16);
    dynamic_array_new(&preview_list_array_bank[2], 16);
    preview_list_has_default = 0;
    preview_list_current_bank = -1;
}
