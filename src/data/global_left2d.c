/* global_left2d @ 0x84176F2C (.data, 4 bytes)
 * DB applied_types: const real_vector2d *global_left2d;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82114530 -> "" (empty string at target)
 * .data data-init (const real_vector2d *).
 * Points at unnamed .rdata constant @ 0x82114530 = {i=0,j=1}.
 */
#include "headers/real_vector2d.h"
static const real_vector2d _global_left2d = { 0.0f, 1.0f };
const real_vector2d *global_left2d = &_global_left2d;
