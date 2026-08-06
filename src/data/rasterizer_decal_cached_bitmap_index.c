/* rasterizer_decal_cached_bitmap_index @ 0x8445D3B8 (.data, 2 bytes)
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0000 (2 trailing bytes)
 */
#include <stdint.h>
/* rasterizer_decal_cached_bitmap_index @ 0x8445D3B8 - .data zero-fill (IDA word_8445D3B8, no PDB name).
 * Halfword: _rasterizer_decals_begin resets it with `sth r10,(word_8445D3B8-...)(r30)` (disasm 0x837110E0,
 * r10 = -1). */

int16_t rasterizer_decal_cached_bitmap_index;
