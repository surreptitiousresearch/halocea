/* s_one_masks @ 0x82129194 (.rdata, 9 bytes)
 * DB applied_types: const unsigned __int8 s_one_masks[9];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000: 00 01 03 07 0F 1F 3F 7F FF
 * low-bit masks indexed by a bit count 0..8:
 * s_one_masks[n] = (1 << n) - 1. Nine bytes; +0x09..+0x0B are zero alignment padding inside the
 * 12-byte slot. read_bits/write_bits/bitstream_read_bits/bitstream_write_bits index it with a
 * chunk size that runs 0..8.
 */
#include <stdint.h>

const uint8_t s_one_masks[9] =
{
    0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF
};
