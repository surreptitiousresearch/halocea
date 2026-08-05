/* transparent_geometry_group_buckets @ 0x84468420 - .data zero-fill (IDA dword_84468420, no PDB name).
 * The 384-bit transparent-geometry group 'pending' bit vector. Size 48 is proven by
 * rasterizer_transparent_geometry_begin's memset (disasm 0x83768338 `addi r3,r11,(dword_84468420-...)` +
 * 0x8376833C `li r5,0x30`), which matches the 0x180 = 384 group cap tested at 0x83768380. */

unsigned char transparent_geometry_group_buckets[48];
