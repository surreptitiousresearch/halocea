/* global_shader_effect_additive @ 0x84186C58, 180 bytes
   (data-init; ledger size 184 overcounts by 4 trailing zero bytes — struct is 180).
   Built-in "additive" shader_effect default. Reconstructed from the raw image
   (big-endian). Differs from global_shader_effect_alpha_blended only in
   effect.framebuffer_blend_function (=3, additive) vs the alpha-blended default.
   The secondary_map.name pointer is the absolute image address baked into the
   .data image (points at non-string image data in the original — reproduced
   verbatim for byte fidelity). */
#include "../headers/shader_effect.h"

shader_effect global_shader_effect_additive = {
    .shader = {
        .type = 1,                       /* offset 36 (0x24) */
    },
    .effect = {
        .framebuffer_blend_function = 3, /* effect offset 2 (abs 0x2A) — additive */
        .secondary_map = {               /* effect offset 36 (abs 0x4C) */
            .group_tag   = 0x6269746Du,  /* 'bitm' */
            .name        = (char *)0x8200155A,
            .name_length = 0,
            .index       = -1,           /* 0xFFFFFFFF */
        },
    },
};
