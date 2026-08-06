/* memQuotaMb @ 0x841440B4 (.data, 4 bytes)
 * DB applied_types: int memQuotaMb;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x000000FA
 * .data data-init. DB symbol ?memQuotaMb@@3HA — MANGLED, so this storage definition must have C++
 * linkage (a C TU cannot emit `?memQuotaMb@@3HA`); hence a .cpp here rather than the usual
 * src/data/<sym>.c, following src/data/IGNORE_STRONG_ASSERT.cpp. `3` = a variable, `H` = int,
 * `A` = plain non-const. It is therefore absent from blam_data_globals.h, which declares the
 * C-linkage globals only.
 * Raw big-endian image bytes 00 00 00 FA = 250, i.e. a 250 MB video-memory quota; vidAllocPhysMem shifts it
 * left by 20 to get bytes (disasm 0x827CDB34 `slwi r7,r10,20`).
 */
int memQuotaMb = 250;
