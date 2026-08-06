/* global_z_axis3d @ 0x84176F44 (.data, 4 bytes)
 * DB applied_types: const real_point3d *global_z_axis3d;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8211453C -> "" (empty string at target)
 * .data data-init (const real_point3d *).
 * Points at .rdata constant @ 0x8211453C = {0,0,1}.
 */
#include "../headers/real_point3d.h"
static const real_point3d _global_z_axis3d = { { 0.0f, 0.0f, 1.0f } };
const real_point3d *global_z_axis3d = &_global_z_axis3d;
