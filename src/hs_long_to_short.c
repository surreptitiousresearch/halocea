/* hs_long_to_short @0x8368D618 — packs a short into the high 16 bits of the return register.
 * DEVIATION: decompiler renders HIWORD(back_chain)=l; on big-endian PPC, sth r3,0(r1) stores
 * to the high halfword of the stack slot and lwz r3,0(r1) reads back the full 32-bit word.
 * DEVIATION: the parameter is the full `int` script value word (the binary truncates r3 with
 * `sth`), not an already-narrowed int16_t — the typecasting_procedures ABI is word-in/word-out.
 * CAVEAT: as-shipped — sth @0x8368D618 writes only the MSWord, lwz @0x8368D61C returns the whole
 * word; the low halfword is whatever sits in the back-chain stack slot (not defined by this code). */
int hs_long_to_short(int l)
{
    int back_chain;
    *(unsigned short *)&back_chain = (unsigned short)l;
    return back_chain;
}
