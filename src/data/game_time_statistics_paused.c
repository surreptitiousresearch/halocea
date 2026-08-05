/* game_time_statistics_paused @ 0x8442096C - .data zero-fill (IDA byte_8442096C, no PDB name).
 * Byte: game_time_start clears it with `stb r9,(byte_8442096C-0x84420968)(r7)` (disasm 0x8369AA0C, r9 = 0),
 * r7 anchored on game_time_statistics (0x84420968). */

unsigned char game_time_statistics_paused;
