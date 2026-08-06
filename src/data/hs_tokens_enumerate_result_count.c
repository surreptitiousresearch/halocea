/* hs_tokens_enumerate_result_count @ 0x8445F6A0 (.data, 8 bytes)
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 */
#include <stdint.h>
/* hs_tokens_enumerate_result_count @ 0x8445F6A0 - .data zero-fill (IDA word_8445F6A0, no PDB name).
 * Halfword: hs_tokens_enumerate zeroes it (`sth`, disasm 0x8372F808) and re-reads it with `lhz` for the
 * qsort element count and the return value (0x8372F874 / 0x8372F894). */

int16_t hs_tokens_enumerate_result_count;
