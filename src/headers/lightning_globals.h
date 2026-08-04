#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
#include "data_array.h"

typedef struct lightning_globals_type
{
    data_array *lightning_data; /* 0x00 */
} lightning_globals_type;

extern lightning_globals_type lightning_globals;
