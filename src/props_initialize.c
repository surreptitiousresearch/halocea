/* props_initialize @ 0x837D1708 — AI prop pool */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void props_initialize(void)
{
    prop_data = game_state_data_new("prop", 768, 312);
}
