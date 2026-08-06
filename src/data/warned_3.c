/* warned_3 @ 0x8446C418 (.data, 1 bytes)
 * DB applied_types: unsigned __int8 warned_3;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 00 (1 trailing byte)
 * data-bss zero-fill; one-shot "pool exhausted" warning latch.
 * DEVIATION: was `int`. The only two accesses in the binary are byte-wide —
 *   0x8378D134  lbz r11, (warned_3 - 0x8446C418)(r30)
 *   0x8378D148  stb r11, (warned_3 - 0x8446C418)(r30)
 * (both in _rasterizer_model_transparent_geometry_submit, which parks &warned_3 in r30 and uses
 * it as a base for neighbouring globals too) — and applied_types agrees: `unsigned __int8`.
 * Big-endian target: the byte the binary touches is byte 0 of the slot, where an `int`
 * declaration put the flag in byte 3. Next named symbol is local_lightmap_ambient_color @ +4,
 * so bytes 1..3 are alignment padding and narrowing cannot clobber a neighbour.
 */
#include <stdint.h>

uint8_t warned_3;
