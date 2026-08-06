/* s_zero_masks @ 0x82129188 (.rdata, 9 bytes)
 * DB applied_types: const unsigned __int8 s_zero_masks[9];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000: FF FE FC F8 F0 E0 C0 80 00
 * the complements of s_one_masks, indexed by a bit count
 * 0..8: s_zero_masks[n] = (uint8_t)~((1 << n) - 1), the mask that clears the low n bits of a
 * destination byte. Nine bytes; +0x09..+0x0B are zero alignment padding inside the 12-byte slot.
 */
#include <stdint.h>

const uint8_t s_zero_masks[9] =
{
    0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00
};
