/* game_time_statistics_recording @ 0x8442096D (.data, 35 bytes)
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x00000000
 *   +0x000C = 0x00000000
 *   +0x0010 = 0x00000000
 *   +0x0014 = 0x00000000
 *   +0x0018 = 0x00000000
 *   +0x001C = 0x00000000
 *   +0x0020 = 000000 (3 trailing bytes)
 * .data zero-fill (IDA byte_8442096D, no PDB name).
 * Byte: game_time_start sets it with `stb r10,(byte_8442096D-0x84420968)(r7)` (disasm 0x8369AA08, r10 = 1).
 */
unsigned char game_time_statistics_recording;
