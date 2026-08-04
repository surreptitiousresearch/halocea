#pragma once
// DB-verified via types_members _8042BC6F7CA5525B1417034C7570FD6D (anonymous PDB type)
/* lights_globals — the dynamic-light system's global state (anonymous struct in the database). Only the
 * fields touched by reconstructed source are modeled. */

#include "rasterizer_lens_flare_submit_parameters.h"

typedef struct lights_globals_t
{
    unsigned __int8 marker_initialized;      /* 0x000 — set while a light-gathering pass is in progress */
    char            _pad001[3];              /* 0x001 */
    int             marker;                  /* 0x004 — counter used to stamp lights visited this pass */
    __int16         scene_point_light_count; /* 0x008 */
    char            _pad00A[2];              /* 0x00A */
    int             scene_point_lights[128]; /* 0x00C */
    rasterizer_lens_flare_submit_parameters queued_lens_flares[8]; /* 0x20C */
    __int16         queued_lens_flare_count; /* 0x34C */
    char            _pad34E[2];              /* 0x34E */
} lights_globals_t;                          /* 848 bytes */
