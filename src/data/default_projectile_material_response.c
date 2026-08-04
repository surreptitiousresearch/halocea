/* default_projectile_material_response @ 0x841855A8, 160 bytes (data-init).
   Built-in default projectile_material_response_definition. Reconstructed from the
   raw image (big-endian). The three tag_reference fields (default_effect,
   possible_response_effect, detonation_effect) each carry a baked 'effe' group
   fourcc, a name pointer that is the absolute .data image address 0x8200155A (the
   shared empty-name image datum other batches reproduce verbatim), name_length 0,
   and index = -1 per the tag_reference convention. All other fields are zero. */
#include "../headers/projectile_material_response_definition.h"

projectile_material_response_definition default_projectile_material_response = {
    .default_effect = {                  /* offset 4 */
        .group_tag   = 0x65666665u,      /* 'effe' */
        .name        = (char *)0x8200155A,
        .name_length = 0,
        .index       = -1,               /* 0xFFFFFFFF */
    },
    .possible_response_effect = {        /* offset 60 (0x3C) */
        .group_tag   = 0x65666665u,      /* 'effe' */
        .name        = (char *)0x8200155A,
        .name_length = 0,
        .index       = -1,
    },
    .detonation_effect = {               /* offset 104 (0x68) */
        .group_tag   = 0x65666665u,      /* 'effe' */
        .name        = (char *)0x8200155A,
        .name_length = 0,
        .index       = -1,
    },
};
