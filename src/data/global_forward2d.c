/* global_forward2d @ 0x84176F28 (.data, 4 bytes)
 * DB applied_types: const real_vector2d *global_forward2d;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82114524 -> private_identity4x3+0x4
 */
#include "../headers/real_vector2d.h"
/* points to anonymous .rdata constant; bytes {1.0, 0.0} */
static const real_vector2d _global_forward2d = { { 1.0f, 0.0f } };
const real_vector2d *global_forward2d = &_global_forward2d;
