/* memQuotaMb @ 0x841440B4 - .data data-init. DB symbol ?memQuotaMb@@3HA (int).
 * Raw big-endian image bytes 00 00 00 FA = 250, i.e. a 250 MB video-memory quota; vidAllocPhysMem shifts it
 * left by 20 to get bytes (disasm 0x827CDB34 `slwi r7,r10,20`). */

int memQuotaMb = 250;
