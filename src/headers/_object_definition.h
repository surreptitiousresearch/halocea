#pragma once
/* _object_definition — the shared tag payload every object-derived definition (biped, vehicle,
 * weapon, equipment, scenery, ...) begins with (380 bytes), per an authoritative structure dump
 * (C:\halo\headers_ref\_object_definition.h). Resolves reads like
 * `TAG_GET(int, *unit_object)[17]` (int index 17 = byte 68) in
 * action_obey_command_begin.c's atom 0xD handler down to `object.animation_graph.index`. */

#include <stdint.h>
#include "real_point3d.h"
#include "tag_reference.h"
#include "tag_block.h"

typedef struct _object_definition
{
    int16_t          type;                        /* 0x00 */
    uint16_t flags;                        /* 0x02 */
    float            bounding_radius;              /* 0x04 */
    real_point3d     bounding_offset;               /* 0x08 */
    real_point3d     origin_offset;                  /* 0x14 */
    float            acceleration_scale;              /* 0x20 */
    unsigned int     runtime_flags;                    /* 0x24 */
    tag_reference    model;                              /* 0x28 */
    tag_reference    animation_graph;                      /* 0x38 — .index is byte 0x44 (68) */
    int              unused3[10];                            /* 0x48 */
    tag_reference    collision_model;                          /* 0x70 */
    tag_reference    physics;                                    /* 0x80 */
    tag_reference    modifier_shader;                              /* 0x90 */
    tag_reference    creation_effect;                                /* 0xA0 */
    int              unused1[21];                                     /* 0xB0 */
    float            render_bounding_radius;                            /* 0x104 */
    int16_t          function_modes[4];                                  /* 0x108 */
    int              unused2[11];                                          /* 0x110 */
    int16_t          icon_text_index;                                        /* 0x13C */
    int16_t          forced_shader_permutation_index;                          /* 0x13E */
    tag_block        attachments;                                                /* 0x140 */
    tag_block        widgets;                                                      /* 0x14C */
    tag_block        functions;                                                      /* 0x158 */
    tag_block        change_colors;                                                    /* 0x164 */
    tag_block        predicted_resources;                                                /* 0x170 */
} _object_definition; /* 0x17C = 380 bytes */
