/* rasterizer_decal_cached_bitmap_group_index @ 0x8445D3B4 - .data zero-fill (IDA dword_8445D3B4, no PDB name).
 * Full word: _rasterizer_decals_begin resets it with `stw r9,(dword_8445D3B4-...)(r30)` (disasm 0x837110E4,
 * r9 = -1) - a tag-instance index, not a datum handle. */

int rasterizer_decal_cached_bitmap_group_index;
