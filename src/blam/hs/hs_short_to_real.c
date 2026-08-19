/* hs_short_to_real @0x8368D568 — HaloScript type coercion: reinterpret a short-typed script value as a
 * real (float). The source short occupies the high 16 bits of the argument word (big-endian lhz).
 * DEVIATION: extsh+fcfid+frsp = sign-extend to 64-bit then convert to 32-bit float; the 64-bit
 * step is a PPC artifact (no direct int32->float path) and is value-identical, so it is not cast.
 * DEVIATION: returns `int`, not `float` — stfs f12 / lwz r3 hands back the float's bit pattern in r3
 * (f1 is never written). See hs_long_to_real for the shared typecasting_procedures ABI. */

int hs_short_to_real(int value)
{
    short source = (short)((unsigned int)value >> 16);
    float real = (float)source;
    int back_chain;
    *(float *)&back_chain = real;
    return back_chain;
}
