#pragma once
/* glow_definition — the "glow" tag definition (glow.c), 340 bytes, DB types_members-confirmed.
 * attachment_marker doubles as the marker-name argument passed to object_get_marker_by_name (glow_update.c) —
 * the definition's own address is used directly as that char* since the name is its first field. */

#include <stdint.h>
#include "real_argb_color.h"
#include "tag_reference.h"

typedef struct glow_definition
{
    char              attachment_marker[32];                                /* 0x00 */
    int16_t           number_of_particles;                                  /* 0x20 */
    int16_t           boundary_effect;                                      /* 0x22 */
    int16_t           normal_particle_distribution;                        /* 0x24 */
    int16_t           trailing_particle_distribution;                      /* 0x26 */
    unsigned int      flags;                                                /* 0x28 */
    int               unused0[7];                                          /* 0x2C */
    int16_t           render_mode;                                          /* 0x48 */
    int16_t           render_orientation;                                   /* 0x4A */
    int               render_flags;                                        /* 0x4C */
    int16_t           particle_rotational_velocity_attachment_index;       /* 0x50 */
    uint16_t  uw0;                                                  /* 0x52 */
    float             glow_particle_rotational_velocity;                   /* 0x54 */
    float             glow_particle_rotational_velocity_scale_lower_bound; /* 0x58 */
    float             glow_particle_rotational_velocity_scale_upper_bound; /* 0x5C */
    int16_t           effect_rotational_velocity_attachment_index;         /* 0x60 */
    uint16_t  uw1;                                                  /* 0x62 */
    float             glow_effect_rotational_velocity;                     /* 0x64 */
    float             glow_effect_rotational_velocity_scale_lower_bound;   /* 0x68 */
    float             glow_effect_rotational_velocity_scale_upper_bound;   /* 0x6C */
    int16_t           effect_translational_velocity_attachment_index;      /* 0x70 */
    uint16_t  uw2;                                                  /* 0x72 */
    float             glow_effect_translational_velocity;                  /* 0x74 */
    float             glow_effect_translational_velocity_scale_lower_bound; /* 0x78 */
    float             glow_effect_translational_velocity_scale_upper_bound; /* 0x7C */
    int16_t           distance_to_object_attachment_index;                  /* 0x80 */
    uint16_t  uw3;                                                  /* 0x82 */
    float             minimum_distance_glow_particle_to_object;            /* 0x84 */
    float             maximum_distance_glow_particle_to_object;            /* 0x88 */
    float             distance_to_object_scale_lower_bound;                 /* 0x8C */
    float             distance_to_object_scale_upper_bound;                 /* 0x90 */
    int               unused2[2];                                          /* 0x94 */
    int16_t           particle_size_attachment_index;                       /* 0x9C */
    uint16_t  uw4;                                                  /* 0x9E */
    float             particle_size_lower_bound;                            /* 0xA0 */
    float             particle_size_upper_bound;                            /* 0xA4 */
    float             particle_size_scale_lower_bound;                      /* 0xA8 */
    float             particle_size_scale_upper_bound;                      /* 0xAC */
    int16_t           color_attachment_index;                               /* 0xB0 */
    uint16_t  uw5;                                                  /* 0xB2 */
    real_argb_color   color_bound_0;                                        /* 0xB4 */
    real_argb_color   color_bound_1;                                        /* 0xC4 */
    real_argb_color   scale_color_0;                                        /* 0xD4 */
    real_argb_color   scale_color_1;                                        /* 0xE4 */
    float             color_rate_of_change;                                 /* 0xF4 */
    float             percentage_edge_fade;                                 /* 0xF8 */
    float             trailing_particle_generation_frequency;               /* 0xFC */
    float             trailing_particle_lifetime;                          /* 0x100 */
    float             trailing_particle_velocity;                          /* 0x104 */
    float             trailing_particle_minimum_normalized_t;               /* 0x108 */
    float             trailing_particle_maximum_normalized_t;               /* 0x10C */
    int               unused1[13];                                         /* 0x110 */
    tag_reference     texture;                                              /* 0x144 */
} glow_definition; /* 340 bytes */
