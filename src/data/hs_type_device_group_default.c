/* hs_type_device_group_default @ 0x82118F98 (.rdata, 2 bytes)
 * DB applied_types: const __int16 hs_type_device_group_default;
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000 = ffff (2 trailing bytes)
 */
#include <stdint.h>
/* hs_type_device_group_default @ 0x82118F98 - .rdata read-only constant. DB symbol name is the leading-underscore
 * form _hs_type_device_group_default (the original file-scope spelling); consumers use the plain name, the same
 * convention src/data/hs_type_string_default.c already established. Access width: lhz; image halfword FFFF.
 * hs_global_reconcile_read substitutes it when an external HaloScript global's backing pointer is null. */

const int16_t hs_type_device_group_default = -1;
