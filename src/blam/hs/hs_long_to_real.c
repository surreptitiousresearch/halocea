/* hs_long_to_real @0x8368D590 — HaloScript type coercion: long script value -> real.
 * DEVIATION: returns `int`, not `float`. The binary builds the float in f12 and then does
 *   stfs f12, back_chain(r1) / lwz r3, back_chain(r1)
 * i.e. it hands back the float's BIT PATTERN in r3 and never writes f1. Every slot of
 * typecasting_procedures[][] shares this word-in / word-out ABI, which is why the table's
 * DB applied type is `int (*)(int)`.
 * DEVIATION: extsw+fcfid+frsp = sign-extend int to 64-bit, convert to float. The 64-bit widening
 * is an artifact of PPC having no direct int32->float path; every int32 is exactly representable
 * as int64, so `(float)l` is the same value and the intermediate cast is not transcribed. */
int hs_long_to_real(int l)
{
    float real = (float)l;
    int back_chain;
    *(float *)&back_chain = real;
    return back_chain;
}
