#pragma once
/* point_light_definition — a dynamic point/spot light tag definition (0x160 bytes). geometry, gel, color,
 * lens_flare (tag-reference field only), radiosity and effect are modeled. */

#include "point_light_geometry_parameters.h"
#include "point_light_gel_parameters.h"
#include "point_light_color_parameters.h"
#include "point_light_lens_flare_parameters.h"
#include "point_light_radiosity_parameters.h"
#include "point_light_effect_parameters.h"

typedef struct point_light_definition
{
    unsigned int                       flags;      /* 0x00 */
    point_light_geometry_parameters    geometry;   /* 0x04 */
    point_light_color_parameters       color;      /* 0x34 */
    point_light_gel_parameters         gel;        /* 0x64 */
    point_light_lens_flare_parameters  lens_flare; /* 0xAC */
    point_light_radiosity_parameters   radiosity;  /* 0xD4 */
    point_light_effect_parameters      effect;     /* 0xF4 */
    int                                unused[23]; /* 0x104 */
} point_light_definition;                          /* 0x160 */
