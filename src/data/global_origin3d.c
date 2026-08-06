/* global_origin3d @ 0x84176F38 (.data, 4 bytes)
 * DB applied_types: const real_point3d *global_origin3d;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82114548 -> "" (empty string at target)
 * data-init pointer (4 bytes) to a {0,0,0} real_point3d
 * constant in .rdata (original target 0x82114548, shared with global_zero_vector3d).
 * Pointee bytes verified big-endian = 0.0,0.0,0.0.
 */
#include "../headers/real_point3d.h"

static const real_point3d _global_origin3d = { { 0.0f, 0.0f, 0.0f } };
const real_point3d *global_origin3d = &_global_origin3d;
