/* rasterizer_globals @ 0x84175980 — data-INIT (96 bytes). Ledger marks this data-bss, but the
 * binary carries nonzero constant defaults; reconstructed field-by-field from the big-endian
 * image (see .data bytes @ 0x84175980). Only nonzero members are listed; the rest zero-init. */
#include "../headers/rasterizer_globals.h"

rasterizer_globals_struct rasterizer_globals = {
    .z_near = 0.0625f,
    .z_far = 1024.0f,
    .z_near_first_person = 0.01171875f,
    .z_far_first_person = 1024.0f,
    .maximum_nodes_per_model = 44,
    .fixed_function_ambient = 4,
    .use_cheap_active_camo = 32,
};
