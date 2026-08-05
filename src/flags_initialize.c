#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void flags_initialize(void)
{
    flag_data = game_state_data_new("flag", 2, 5820);
}
