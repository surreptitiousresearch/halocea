#pragma once
#include <stdint.h>
/* effects_information — snapshot of the effect subsystem's occupancy, filled by effects_information_get. */

typedef struct effects_information
{
    int16_t effect_count;         /* 0x00 — effect_data->actual_count */
    int16_t location_count;       /* 0x02 — effect_location_data->actual_count */
    int16_t active_effect_count;  /* 0x04 — effects not disabled/hidden */
} effects_information;
