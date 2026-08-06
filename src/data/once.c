/* once @ 0x84184CC4 (.data, 2 bytes)
 * DB applied_types: __int16 once;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0001 (2 trailing bytes)
 * .data-initialized one-shot latch, read and cleared by
 * structure_render_build_dynamic_triangles when rasterizer_dynamic_triangles_new fails.
 * DEVIATION: was `uint8_t once;` (zero-init), on the theory that "the defining byte is 0x00 and
 * the following 0x01 belongs to a separate adjacent global". Both halves of that are wrong:
 *  - there is no adjacent global at +1 — the next named symbol is packet_header_bs_codes @ +4,
 *    so bytes 2..3 are alignment padding;
 *  - the binary accesses the object as a SIGNED HALFWORD, not a byte:
 *      0x837C6590  lhz   r11, once@l(r10)
 *      0x837C6594  extsh r9, r11
 *      0x837C65A4  sth   r11, once@l(r10)
 *    so bytes 0..1 are one 16-bit object, and on this big-endian target its image value is the
 *    halfword 0x0001 = 1, not 0. applied_types agrees: `__int16 once;`.
 * Initialized to 1 (the latch starts armed); the release build's branchless codegen for the
 * once-only body clears it to 0 on the first failure.
 */
#include <stdint.h>

int16_t once = 1;
