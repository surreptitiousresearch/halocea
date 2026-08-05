#pragma once
/* _shader_transparent_chicago — the type-specific body of a shader_transparent_chicago tag (68 bytes,
 * DB types_members-confirmed). Follows the 40-byte _shader base inside shader_transparent_chicago. */

#include <stdint.h>
#include "tag_reference.h"
#include "tag_block.h"

typedef struct _shader_transparent_chicago
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
    int             extra_flags;                 /* 0x38 */
    int             unused[2];                   /* 0x3C */
} _shader_transparent_chicago;                   /* 0x44 = 68 bytes */
