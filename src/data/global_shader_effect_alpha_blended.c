/* global_shader_effect_alpha_blended @ 0x84186D10, 180 bytes
   (data-init; ledger mislabeled data-bss).
   Built-in "alpha blended" shader_effect default. Only a handful of fields are
   non-zero in the binary; reconstructed from the raw image (big-endian). The
   secondary_map.name pointer is the absolute image address baked into the .data
   image (points at non-string image data in the original — reproduced verbatim
   for byte fidelity). */
#include "../headers/shader_effect.h"

shader_effect global_shader_effect_alpha_blended = {
    .shader = {
        .type = 1,                       /* offset 36 */
    },
    .effect = {
        .secondary_map = {               /* effect offset 36 (abs 76) */
            .group_tag   = 0x6269746Du,  /* 'bitm' */
            .name        = (char *)0x8200155A,
            .name_length = 0,
            .index       = -1,           /* 0xFFFFFFFF */
        },
    },
};
