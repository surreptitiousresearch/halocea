#pragma once
/* point_light_lens_flare_parameters — the lens-flare-attachment block of a point_light_definition
 * (0x28 bytes). Only the tag reference (used to resolve the flare's own point_light_definition-shaped
 * flashlight-override tag) is modeled; the rest is left opaque. */

#include "tag_reference.h"

typedef struct point_light_lens_flare_parameters
{
    tag_reference reference;        /* 0x00 */
    int unused[6];                  /* 0x10 */
} point_light_lens_flare_parameters; /* 0x28 */
