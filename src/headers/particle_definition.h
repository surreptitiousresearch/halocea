#pragma once
/* particle_definition — a particle type's tag data (356 bytes, DB layout). */

#include <stdint.h>
#include "tag_reference.h"
#include "shader_effect.h"

typedef struct particle_definition
{
    unsigned int  flags;                                 /* 0x00 */
    tag_reference bitmap;                                /* 0x04 */
    tag_reference physics;                                /* 0x14 */
    tag_reference collision_material_effects;             /* 0x24 */
    int           unused0[1];                              /* 0x34 */
    float         life_span_lower_bound;                    /* 0x38 */
    float         life_span_upper_bound;                     /* 0x3C */
    float         fade_in_time;                               /* 0x40 */
    float         fade_out_time;                               /* 0x44 */
    tag_reference collision_effect;                              /* 0x48 */
    tag_reference death_effect;                                   /* 0x58 */
    float         minimum_pixels;                                  /* 0x68 */
    int           unused2[2];                                       /* 0x6C */
    float         initial_radius_modifier;                           /* 0x74 */
    float         final_radius_modifier;                              /* 0x78 */
    int           unused3[1];                                          /* 0x7C */
    float         frames_per_second_lower_bound;                        /* 0x80 */
    float         frames_per_second_upper_bound;                         /* 0x84 */
    float         frames_per_second_contact_deterioration;                /* 0x88 */
    float         lod_falloff;                                            /* 0x8C */
    float         lod_cutoff;                                              /* 0x90 */
    int           unused4[1];                                               /* 0x94 */
    int16_t       first_sequence_index;                                     /* 0x98 */
    int16_t       initial_sequence_count;                                    /* 0x9A */
    int16_t       looping_sequence_count;                                     /* 0x9C */
    int16_t       final_sequence_count;                                       /* 0x9E */
    int           unused5[2];                                                   /* 0xA0 */
    float         runtime_oo_width;                                              /* 0xA8 */
    int16_t       sprite_orientation;                                            /* 0xAC */
    uint16_t pad;                                                         /* 0xAE */
    shader_effect shader;                                                          /* 0xB0 */
} particle_definition;                                                              /* 356 bytes */
