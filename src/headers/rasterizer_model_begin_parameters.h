#pragma once
/* rasterizer_model_begin_parameters — the full description of a model draw handed to rasterizer_model_begin /
 * rasterizer_environment_shadow_model_begin (built by render_model). Layout from the database (204 bytes). */

#include "render_skinning.h"
#include "render_lighting.h"
#include "render_animation.h"
#include "render_model_effect.h"
#include "real_point3d.h"
#include "real_vector2d.h"

typedef struct rasterizer_model_begin_parameters
{
    unsigned int        geometry_flags; /* 0x00 */
    unsigned int        unique_id;      /* 0x04 */
    render_skinning     skinning;       /* 0x08 */
    render_lighting     lighting;       /* 0x10 (16) */
    render_animation    animation;      /* 0x84 (132) */
    render_model_effect effect;         /* 0x8C (140) */
    real_point3d        centroid;       /* 0xB4 (180) */
    float               radius;         /* 0xC0 (192) */
    real_vector2d       base_map_scale; /* 0xC4 (196) */
} rasterizer_model_begin_parameters;    /* 0xCC (204) */
