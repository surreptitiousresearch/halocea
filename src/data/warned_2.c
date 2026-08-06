/* warned_2 @ 0x8446C0F4 (.data, 1 bytes)
 * DB applied_types: unsigned __int8 warned_2;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 00 (1 trailing byte)
 * data-bss zero-fill; one-shot "pool exhausted" warning latch.
 * DEVIATION: was `int`. The only two accesses in the binary are byte-wide —
 *   0x83786084  lbz r10, warned_2@l(r11)
 *   0x83786094  stb r10, warned_2@l(r11)
 * (both in _rasterizer_widget_submit) — and applied_types agrees: `unsigned __int8 warned_2;`.
 * This is a big-endian target, so the byte the binary touches is byte 0 of the slot; an `int`
 * declaration put the flag in byte 3 instead. The next named symbol is vertex_data_0 @ +4, so
 * bytes 1..3 are alignment padding owned by nobody and narrowing cannot clobber a neighbour.
 */
#include <stdint.h>

uint8_t warned_2;
