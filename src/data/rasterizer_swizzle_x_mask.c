/* rasterizer_swizzle_x_mask @ 0x844A61B4 - .data zero-fill (IDA dword_844A61B4, no PDB name).
 * The X bit-interleave mask, stored between the DB-named siblings az (0x844A61B0) and ay (0x844A61B8);
 * compute_swizzle_masks writes all three with `stw` (disasm 0x837E30C8 / 0x837E3124). */

unsigned int rasterizer_swizzle_x_mask;
