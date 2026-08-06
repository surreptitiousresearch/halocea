/* transparent_geometry_group_buckets @ 0x84468420 (.data, 48 bytes)
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x00000000
 *   +0x000C = 0x00000000
 *   +0x0010 = 0x00000000
 *   +0x0014 = 0x00000000
 *   +0x0018 = 0x00000000
 *   +0x001C = 0x00000000
 *   +0x0020 = 0x00000000
 *   +0x0024 = 0x00000000
 *   +0x0028 = 0x00000000
 *   +0x002C = 0x00000000
 * .data zero-fill (IDA dword_84468420, no PDB name).
 * The 384-bit transparent-geometry group 'pending' bit vector. Size 48 is proven by
 * rasterizer_transparent_geometry_begin's memset (disasm 0x83768338 `addi r3,r11,(dword_84468420-...)` +
 * 0x8376833C `li r5,0x30`), which matches the 0x180 = 384 group cap tested at 0x83768380.
 */
unsigned char transparent_geometry_group_buckets[48];
