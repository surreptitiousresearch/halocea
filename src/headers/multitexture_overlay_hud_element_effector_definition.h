#pragma once
/* multitexture_overlay_hud_element_effector_definition — 220-byte scripted HUD
 * overlay effector record (DB-verified layout). One drives a map/master output
 * channel from an input source over an interpolation range. */

#include "real_rgb_color.h"

typedef struct multitexture_overlay_hud_element_effector_definition
{
    int unused1[16];                        /* +0 */
    __int16 destination_type;               /* +64  (code: output_channel) */
    __int16 destination;                    /* +66  (code: output_type / map select) */
    __int16 source;                         /* +68  (code: input_type) */
    unsigned __int16 pad1;                  /* +70 */
    float in_bounds[2];                     /* +72  input lower/upper */
    float out_bounds[2];                    /* +80  output scalar lower/upper */
    int unused2[16];                        /* +88 */
    real_rgb_color tint_color_lower_bounds; /* +152 */
    real_rgb_color tint_color_upper_bounds; /* +164 */
    __int16 periodic_function;              /* +176 */
    unsigned __int16 pad2;                  /* +178 */
    float periodic_function_period;         /* +180 */
    float periodic_function_phase;          /* +184 */
    int unused3[8];                         /* +188 */
} multitexture_overlay_hud_element_effector_definition;
