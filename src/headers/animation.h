#pragma once
/* animation — one animation in a model_animations tag's animations block (180-byte element). Full field layout
 * from the database. The three per-node channel bitflag arrays (rotation/translation/scale) are unsigned int[2],
 * i.e. up to 64 nodes; bit N selects whether node N is animated (data in the frame stream) vs. default. */

#include <stdint.h>
#include "tag_data.h"

typedef struct animation
{
    char             name[0x20];                    /* 0x00 */
    int16_t          type;                           /* 0x20 */
    int16_t          frame_count;                    /* 0x22 */
    int16_t          frame_size;                     /* 0x24 */
    int16_t          frame_info_type;                /* 0x26 */
    unsigned int     node_list_checksum;             /* 0x28 */
    int16_t          node_count;                     /* 0x2C */
    int16_t          private_loop_frame_index;       /* 0x2E */
    float            weight;                          /* 0x30 */
    int16_t          private_key_frame_index;         /* 0x34 */
    int16_t          private_second_key_frame_index;  /* 0x36 */
    int16_t          next_animation_index;            /* 0x38 */
    uint16_t flags;                           /* 0x3A — bit0 (0x1) = compressed-data present */
    int16_t          sound_index;                     /* 0x3C */
    int16_t          private_sound_frame_index;       /* 0x3E */
    uint8_t  private_left_foot_frame_index;   /* 0x40 */
    uint8_t  private_right_foot_frame_index;  /* 0x41 */
    int16_t          runtime_parent_animation_index;  /* 0x42 */
    float            runtime_normalized_weight;       /* 0x44 */
    tag_data         frame_info;                      /* 0x48 — per-frame movement (dx/dy/dyaw/...) */
    unsigned int     nodes_with_translation_flags[2]; /* 0x5C */
    int              unused_translation[2];           /* 0x64 */
    unsigned int     nodes_with_rotation_flags[2];    /* 0x6C */
    int              unused_rotation[2];              /* 0x74 */
    unsigned int     nodes_with_scale_flags[2];       /* 0x7C */
    int              unused_scale[1];                 /* 0x84 */
    int              compressed_data_offset;          /* 0x88 */
    tag_data         default_data;                    /* 0x8C — per-node default pose (non-animated channels) */
    tag_data         data;                            /* 0xA0 — frame data block */
} animation;                                          /* 0xB4 = 180 bytes */
