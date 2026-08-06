/* default_projectile_material_response @ 0x841855A8 (.data, 160 bytes)
 * DB applied_types: projectile_material_response_definition default_projectile_material_response;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 flags                      = 0x0000
 *   +0x02 default_response           = 0x0000
 *   +0x04 default_effect             = 0x656666658200155A00000000FFFFFFFF
 *   +0x14 unused0                    = 0x00000000000000000000000000000000
 *   +0x24 possible_response          = 0x0000
 *   +0x26 possible_response_flags    = 0x0000
 *   +0x28 possible_response_skip_fraction = 0x00000000 -> 0f
 *   +0x2C possible_response_minimum_angle = 0x00000000 -> 0f
 *   +0x30 possible_response_maximum_angle = 0x00000000 -> 0f
 *   +0x34 possible_response_minimum_velocity = 0x00000000 -> 0f
 *   +0x38 possible_response_maximum_velocity = 0x00000000 -> 0f
 *   +0x3C possible_response_effect   = 0x656666658200155A00000000FFFFFFFF
 *   +0x4C unused1                    = 0x00000000000000000000000000000000
 *   +0x5C scale_effects_by           = 0x0000
 *   +0x5E pad                        = 0x0000
 *   +0x60 angle_noise                = 0x00000000 -> 0f
 *   +0x64 velocity_noise             = 0x00000000 -> 0f
 *   +0x68 detonation_effect          = 0x656666658200155A00000000FFFFFFFF
 *   +0x78 unused2                    = 0x000000000000000000000000000000000000000000000000
 *   +0x90 penetration_initial_friction = 0x00000000 -> 0f
 *   +0x94 penetration_maximum_distance = 0x00000000 -> 0f
 *   +0x98 reflection_parallel_friction = 0x00000000 -> 0f
 *   +0x9C reflection_perpendicular_friction = 0x00000000 -> 0f
 * 160 bytes (data-init).
 *    Built-in default projectile_material_response_definition. Reconstructed from the
 *    raw image (big-endian). The three tag_reference fields (default_effect,
 *    possible_response_effect, detonation_effect) each carry a baked 'effe' group
 *    fourcc, a name pointer that is the absolute .data image address 0x8200155A (the
 *    shared empty-name image datum other batches reproduce verbatim), name_length 0,
 *    and index = -1 per the tag_reference convention. All other fields are zero.
 */
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
