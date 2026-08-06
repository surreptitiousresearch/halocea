/* global_null_rectangle2d @ 0x84176F84 (.data, 4 bytes)
 * DB applied_types: const real_rectangle2d *global_null_rectangle2d;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8211459C -> private_null_rectangle
 * .data data-init (const real_rectangle2d *).
 * Points at .rdata inverted-empty box @ 0x8211459C = {x0=+FLT_MAX,x1=-FLT_MAX,y0=+FLT_MAX,y1=-FLT_MAX}.
 */
#include "../headers/real_rectangle2d.h"
static const real_rectangle2d _global_null_rectangle2d = {
    { 3.40282347e+38f, -3.40282347e+38f, 3.40282347e+38f, -3.40282347e+38f }
};
const real_rectangle2d *global_null_rectangle2d = &_global_null_rectangle2d;
