/* sALL_ONES @ 0x82124FC4 (.rdata, 4 bytes)
 * DB applied_types: const int sALL_ONES;
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000 = 0xFFFFFFFF
 * 0xFFFFFFFF, the all-ones bit source
 * default_smart_vector_encoder passes by address to bitstream_write_bits to emit a run of
 * unary one-bits. Exactly 4 bytes (next .rdata symbol is at +0x04).
 */
#include <stdint.h>

const uint32_t sALL_ONES = 0xFFFFFFFFu;
