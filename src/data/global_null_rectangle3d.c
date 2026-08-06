/* global_null_rectangle3d @ 0x84176F88 (.data, 4 bytes)
 * DB applied_types: const real_rectangle3d *global_null_rectangle3d;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8211459C -> private_null_rectangle
 * .data data-init (const real_rectangle3d *).
 * DB decl is a 4-byte pointer; ledger size 8 includes 4 trailing zero bytes (separate global).
 * Points at unnamed .rdata constant @ 0x8211459C = inverted-empty box
 * {x0=+FLT_MAX,x1=-FLT_MAX,y0=+FLT_MAX,y1=-FLT_MAX,z0=+FLT_MAX,z1=-FLT_MAX}.
 */
#include "headers/real_rectangle3d.h"
static const real_rectangle3d _global_null_rectangle3d = {
    3.40282347e+38f, -3.40282347e+38f,
    3.40282347e+38f, -3.40282347e+38f,
    3.40282347e+38f, -3.40282347e+38f
};
const real_rectangle3d *global_null_rectangle3d = &_global_null_rectangle3d;
