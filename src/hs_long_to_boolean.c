/* DEVIATION: cntlzw/extrwi/stb r10,back_chain(r1)/lwz r3,back_chain(r1) on big-endian PPC:
 * stores (n==0)?1:0 into the MSB of the back-chain stack slot, then reads back the full word.
 * DEVIATION: the parameter is the plain `int` script value word (typecasting_procedures is
 * uniformly word-in/word-out); the zero test is sign-agnostic. */
int hs_long_to_boolean(int n)
{
    int back_chain;
    *(unsigned char *)&back_chain = (n == 0) ? 1 : 0;
    return back_chain;
}
