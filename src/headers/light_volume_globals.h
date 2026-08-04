#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
#include "data_array.h"

typedef struct light_volume_globals_type
{
    data_array *light_volume_data; /* 0x00 */
} light_volume_globals_type;

extern light_volume_globals_type light_volume_globals;
