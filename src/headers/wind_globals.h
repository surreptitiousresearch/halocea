/* wind_globals — global wind simulation state (wind.c). One wind_state per weather palette entry of
 * the current structure BSP. */
#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */

#include <stdint.h>
#include "real_vector3d.h"
#include "wind_state.h"

typedef struct wind_globals_data
{
    uint8_t initialized;          /* 0x000 */
    real_vector3d   variance[3][64];      /* 0x004 — precomputed variance noise table */
    int16_t         count;                /* 0x904 — number of active wind states */
    wind_state      wind_states[32];      /* 0x908 */
    int             time;                 /* 0xD08 — tick counter */
} wind_globals_data;                      /* 3340 bytes */

extern wind_globals_data wind_globals;
