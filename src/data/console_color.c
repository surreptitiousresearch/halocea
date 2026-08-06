/* console_color @ 0x84177D44 (.data, 16 bytes)
 * DB applied_types: real_argb_color console_color;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 n                          = 0x3F8000003F8000003E99999A3F800000
 *   +0x00 __s1                       = 0x3F8000003F8000003E99999A3F800000
 *   +0x00 __s2                       = 0x3F8000003F8000003E99999A3F800000
 * .data data-init (real_argb_color). 16-byte value
 * {a=1,r=1,g=0.3,b=1}. Ledger size 20 overcounts by 4 trailing zero bytes (separate global).
 */
#include "../headers/real_argb_color.h"
real_argb_color console_color = { 1.0f, 1.0f, 0.3f, 1.0f };
