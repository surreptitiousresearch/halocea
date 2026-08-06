/* swizzle_table @ 0x82128260 (.rdata, 128 bytes)
 * DB applied_types: const unsigned __int16 swizzle_table[64];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000: 0000 0001 0004 0005 0010 0011 0014 0015
 *   +0x0010: 0040 0041 0044 0045 0050 0051 0054 0055
 *   +0x0020: 0100 0101 0104 0105 0110 0111 0114 0115
 *   +0x0030: 0140 0141 0144 0145 0150 0151 0154 0155
 *   +0x0040: 0400 0401 0404 0405 0410 0411 0414 0415
 *   +0x0050: 0440 0441 0444 0445 0450 0451 0454 0455
 *   +0x0060: 0500 0501 0504 0505 0510 0511 0514 0515
 *   +0x0070: 0540 0541 0544 0545 0550 0551 0554 0555
 * .rdata bit-spread (Morton/Z-order interleave) table, 128 bytes.
 * Element width: every access in bitmap_swizzle_vector2d is an `lhzx` at byte offset 2*index
 * (0x837E31B4/31B8/31F0/31F4/31F8/3200), and the object spans exactly 128 bytes (next .rdata name
 * face_mapping_inverse_table @0x821282E0) — so 64 x uint16_t, matching the corpus
 * `extern const uint16_t swizzle_table[64]`.
 * Content: entry i is i's low 6 bits spread into the even bit positions, i.e.
 *   swizzle_table[i] = sum over k in 0..5 of ((i >> k) & 1) << (2*k).
 * That identity was verified against all 64 dumped halfwords, so the literals below are the
 * binary's own bytes, not a regenerated formula. Max entry 0x0555 (i = 63).
 * /
 */
#include <stdint.h>

const uint16_t swizzle_table[64] =
{
    0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015,   /*  0..7  */
    0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055,   /*  8..15 */
    0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115,   /* 16..23 */
    0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155,   /* 24..31 */
    0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415,   /* 32..39 */
    0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455,   /* 40..47 */
    0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515,   /* 48..55 */
    0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555,   /* 56..63 */
};
