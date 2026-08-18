/* hs_long_to_boolean @0x8368D4F0
 * DEVIATION: cntlzw/extrwi/stb r10,back_chain(r1)/lwz r3,back_chain(r1) on big-endian PPC:
 * stores (n==0)?1:0 into the MSB of the back-chain stack slot, then reads back the full word.
 * DEVIATION: the parameter is the plain `int` script value word (typecasting_procedures is
 * uniformly word-in/word-out); the zero test is sign-agnostic.
 * CAVEAT: as-shipped — stb @0x8368D4F8 writes only the MSByte, lwz @0x8368D4FC returns the whole
 * word, so the shipped low 3 bytes are whatever sat in the back-chain stack slot: indeterminate, not
 * defined by this code. Zero is the portable choice reproduced here; consumers read value >> 24. */
int hs_long_to_boolean(int n)
{
    /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(unsigned char*)&back_chain
     * pun; hs_inspect_boolean and hs_cast's boolean consumers extract value >> 24) 2026-08-18 */
    return (int)((unsigned char)((n == 0) ? 1 : 0)) << 24;
}
