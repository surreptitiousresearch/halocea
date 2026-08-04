#pragma once
/* render_model_effect — per-render-call model effect descriptor (screen-effect / camouflage modifier).
 * Layout from the database (40 bytes). */

#include "real_point3d.h"
#include "render_animation.h"

struct shader;

typedef struct render_model_effect
{
    __int16              type;                   /* 0x00 */
    unsigned char        _pad02[2];              /* 0x02 */
    float                intensity;              /* 0x04 */
    float                parameter;              /* 0x08 */
    int                  source_object_index;    /* 0x0C */
    real_point3d         source_object_centroid; /* 0x10 */
    const struct shader *modifier_shader;        /* 0x1C */ /* const: every writer assigns a const tag shader or NULL; all readers take const shader* 2026-07-31 (C4090) */
    render_animation     modifier_animation;     /* 0x20 */
} render_model_effect;                           /* 0x28 */
