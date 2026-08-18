/* hs_short_to_boolean @0x8368D508
 * DEVIATION: disasm tests sign-extension bits: non-negative short -> 1 (MSByte), negative -> 0
 * CAVEAT: as-shipped — stb @0x8368D51C writes only the MSByte, lwz @0x8368D520 returns the whole
 * word, so the shipped low 3 bytes are whatever sat in the back-chain stack slot: indeterminate, not
 * defined by this code. Zero is the portable choice reproduced here; every consumer reads value >> 24. */
int hs_short_to_boolean(int s)
{
    /* PPC big-endian: lhz loads upper 16 bits of stored int (the sign-extension);
       cntlzw(0)==32 -> bit5==1 -> TRUE for non-negative; negative gives cntlzw(0xFFFF...)==0 -> FALSE */
    /* DEVIATION: `(cntlzw(x) & 0x20) != 0` is exactly `x == 0` (cntlzw returns 0..32, and 32 == 0x20
     * is the only value in that range with bit 5 set), so the count is reduced to the comparison it
     * implements — the same reduction the rest of the corpus applies to this catalog idiom. No
     * __builtin_clz: it is a GCC builtin nothing declares, and the implicit `int()` MSVC invented
     * for it is what the C4013 promotion exists to reject. */
    short upper = (short)(s >> 16);
    /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(unsigned char*)&result pun;
     * consumers extract value >> 24) 2026-08-18 */
    return (int)((unsigned char)(upper == 0)) << 24;
}
