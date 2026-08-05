#pragma once
/* _shader_transparent_chicago_extended — the type-specific body of a shader_transparent_chicago_extended
 * tag (80 bytes, DB types_members-confirmed). Follows the 40-byte _shader base. Identical to
 * _shader_transparent_chicago plus an extra maps_ff tag_block for fixed-function fallback maps. */

#include <stdint.h>
#include "tag_reference.h"
#include "tag_block.h"

typedef struct _shader_transparent_chicago_extended
{
    uint8_t numeric_counter_limit;      /* 0x00 */
    uint8_t flags;                       /* 0x01 */
    int16_t         type;                        /* 0x02 */
    int16_t         framebuffer_blend_function;  /* 0x04 */
    int16_t         framebuffer_fade_mode;       /* 0x06 */
    int16_t         framebuffer_fade_source;     /* 0x08 */
    int16_t         framebuffer_fade_unused;     /* 0x0A */
    float           lens_flare_spacing;          /* 0x0C */
    tag_reference   lens_flare;                  /* 0x10 */
    tag_block       extra_layers;                /* 0x20 */
    tag_block       maps;                        /* 0x2C — per-stage shader_transparent_chicago_map[] */
    tag_block       maps_ff;                     /* 0x38 */
    int             extra_flags;                 /* 0x44 */
    int             unused[2];                   /* 0x48 */
} _shader_transparent_chicago_extended;          /* 0x50 = 80 bytes */
