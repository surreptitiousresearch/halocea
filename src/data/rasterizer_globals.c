/* rasterizer_globals @ 0x84175980 (.data, 96 bytes)
 * DB applied_types: rasterizer_globals_struct rasterizer_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 active                     = 0x00
 *   +0x02 current_lock_operation     = 0x0000
 *   +0x04 screen_bounds              = 0x0000000000000000
 *   +0x0C frame_bounds               = 0x0000000000000000
 *   +0x18 frame_index                = 0x0000000000000000
 *   +0x20 flip_index                 = 0x00000000
 *   +0x24 use_floating_point_zbuffer = 0x00
 *   +0x25 use_rasterizer_frame_rate_throttle = 0x00
 *   +0x26 use_rasterizer_frame_rate_stabilization = 0x00
 *   +0x28 refresh_rate               = 0x0000
 *   +0x2C z_near                     = 0x3D800000 -> 0.0625f
 *   +0x30 z_far                      = 0x44800000 -> 1024f
 *   +0x34 z_near_first_person        = 0x3C400000 -> 0.0117188f
 *   +0x38 z_far_first_person         = 0x44800000 -> 1024f
 *   +0x3C default_white_hardware_format = 0x00000000
 *   +0x40 default_2d_hardware_format = 0x00000000
 *   +0x44 default_3d_hardware_format = 0x00000000
 *   +0x48 default_cm_hardware_format = 0x00000000
 *   +0x4C lightmap_mode              = 0x0000
 *   +0x4E maximum_nodes_per_model    = 0x002C
 *   +0x50 max_simultaneous_textures  = 0x0000
 *   +0x54 fixed_function_ambient     = 0x00000004
 *   +0x58 use_cheap_active_camo      = 0x20
 *   +0x59 render_targets_disabled    = 0x00
 *   +0x5A alpha_render_targets_disabled = 0x00
 * data-INIT (96 bytes). Ledger marks this data-bss, but the
 * binary carries nonzero constant defaults; reconstructed field-by-field from the big-endian
 * image (see .data bytes @ 0x84175980). Only nonzero members are listed; the rest zero-init.
 */
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
