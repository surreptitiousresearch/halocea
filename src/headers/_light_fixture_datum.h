#pragma once

/* _light_fixture_datum — the light-fixture-specific runtime extension of a light_fixture object
 * datum (at composite offset 532). DB-verified layout (types_members _light_fixture_datum). */

#include "real_rgb_color.h"

typedef struct _light_fixture_datum
{
    real_rgb_color color;         /* 0x00 */
    float          intensity;     /* 0x0C (12) */
    float          falloff_angle; /* 0x10 (16) */
    float          cutoff_angle;  /* 0x14 (20) */
} _light_fixture_datum;
