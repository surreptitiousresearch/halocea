#pragma once
/* effects_information — snapshot of the effect subsystem's occupancy, filled by effects_information_get. */

typedef struct effects_information
{
    __int16 effect_count;         /* 0x00 — effect_data->actual_count */
    __int16 location_count;       /* 0x02 — effect_location_data->actual_count */
    __int16 active_effect_count;  /* 0x04 — effects not disabled/hidden */
} effects_information;
