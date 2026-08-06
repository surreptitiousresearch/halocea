/* hs_external_global_count @ 0x82122214 (.rdata, 2 bytes)
 * DB applied_types: const __int16 hs_external_global_count;
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000 = 01e2 (2 trailing bytes)
 * 0x01E2 = 482, the number of entries in
 * hs_external_globals[] (src/data/hs_external_globals.c defines hs_global_external *[482]).
 * A 16-bit value at +0x00 of a 4-byte slot; +0x02..+0x03 are zero alignment padding. Read as a
 * signed short: hs_find_global_by_name @0x8372EF70 does `lhz` followed by `extsh`.
 */
#include <stdint.h>

const int16_t hs_external_global_count = 482;
