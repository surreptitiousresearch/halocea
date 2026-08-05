#pragma once
/* model — model tag (gbxmodel/model). Full header now reconciled against the
 * authoritative reference; the LOD/node-count and unused header fields that were
 * previously opaque padding are named at their true offsets. */

#include <stdint.h>
#include "tag_block.h"
#include "real_vector2d.h"

typedef struct model
{
    unsigned int    flags;                   /* 0x00 */
    unsigned int    node_list_checksum;      /* 0x04 — must equal 124371095 for the Cortana shader hack */
    float           detail_cutoff_pixels[5]; /* 0x08 — LOD pixel-size thresholds (superhigh..superlow) */
    int16_t         node_counts[5];          /* 0x1C — node count per detail level */
    uint16_t pad;                    /* 0x26 */
    int             unused1[2];              /* 0x28 */
    real_vector2d   base_map_scale;          /* 0x30 */
    int             unused2[29];             /* 0x38 */
    tag_block       markers;                 /* 0xAC — model_marker[] */
    tag_block       nodes;                   /* 0xB8 — each node is 156 bytes: +32 first child, +34 next sibling,
                                              *        +36 parent (all u16 node indices, 0xFFFF = none) */
    tag_block       regions;                 /* 0xC4 — model_region[], 76 bytes each */
    tag_block       geometries;              /* 0xD0 — model_geometry[], 48 bytes each */
    tag_block       shaders;                 /* 0xDC — model_shader_reference[], 32 bytes each */
} model; /* 232 bytes */
