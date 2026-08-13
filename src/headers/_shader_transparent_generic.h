#pragma once
/* _shader_transparent_generic — the type-specific body of a shader_transparent_generic tag (68 bytes,
 * DB types_members-confirmed). Follows the 40-byte _shader base inside shader_transparent_generic.
 * Byte-identical prefix to _shader_transparent_chicago through `maps`; the tail block is `stages`
 * (per-stage generic shader stages) rather than chicago's extra_flags/maps_ff. */

#include <stdint.h>
#include "tag_reference.h"
#include "tag_block.h"
#include "shader_transparent_generic_type.h"   /* the domain of `type` @ 0x02 */

typedef struct _shader_transparent_generic
{
    uint8_t         numeric_counter_limit;      /* 0x00 */
    uint8_t         flags;                       /* 0x01 — shader_transparent_generic_flags bits */
    int16_t         type;                        /* 0x02 — shader_transparent_generic_type (first map type); shader_transparent_generic_type.h, DB $9FCCCEA89881530A1DDE69E22F470E0D. Sole image read: shader_get_vertex_shader_permutation @0x837556BC (lhz 0x2A, +1 bias) */
    int16_t         framebuffer_blend_function;  /* 0x04 */
    int16_t         framebuffer_fade_mode;       /* 0x06 */
    int16_t         framebuffer_fade_source;     /* 0x08 */
    int16_t         framebuffer_fade_unused;     /* 0x0A */
    float           lens_flare_spacing;          /* 0x0C */
    tag_reference   lens_flare;                  /* 0x10 */
    tag_block       extra_layers;                /* 0x20 */
    tag_block       maps;                        /* 0x2C */
    tag_block       stages;                      /* 0x38 */
} _shader_transparent_generic;                   /* 0x44 = 68 bytes */
