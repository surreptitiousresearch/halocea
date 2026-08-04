#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* particle_system_particle_state_render_info — the shader/rendering-relevant subset of one 376-byte
 * "particle state" tag element (particle_system_type.particle_states[state_index]). No DB type covers
 * the full 376-byte element; only the fields touched by particle_system_render are modeled here, at
 * their true offsets, with the remainder left as padding per the established convention for this tag
 * family.
 *
 * `blend_function_id`/`fade_mode_id` are read directly at these absolute offsets, but they coincide
 * exactly with `shader.effect.framebuffer_blend_function`/`framebuffer_fade_mode` (shader_effect.h) once
 * `shader` is laid out at 184 — i.e. the two names refer to the SAME bytes, kept separate here only
 * because this function reads them as bare integers rather than through the nested shader_effect. */

#include "shader_effect.h"

typedef struct particle_system_particle_state_render_info
{
    char             _pad00[60];   /* 0x00 */
    int              bitmap_group_index; /* 0x3C (60) */
    __int16          sequence_range;     /* 0x40 (64) — build_sprite's sequence_index /
                                         *  build_sprite_rotational's first_sequence_index */
    char             _pad42[128 - 0x42]; /* 0x42 */
    float            secondary_map_radius; /* 0x80 (128) */
    char             _pad84[184 - 0x84]; /* 0x84 */
    shader_effect    shader;             /* 0xB8 (184) */
} particle_system_particle_state_render_info;
