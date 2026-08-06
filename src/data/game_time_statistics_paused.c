/* game_time_statistics_paused @ 0x8442096C (.data, 36 bytes)
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
 * .data zero-fill (IDA byte_8442096C, no PDB name).
 * Byte: game_time_start clears it with `stb r9,(byte_8442096C-0x84420968)(r7)` (disasm 0x8369AA0C, r9 = 0),
 * r7 anchored on game_time_statistics (0x84420968).
 */
unsigned char game_time_statistics_paused;
