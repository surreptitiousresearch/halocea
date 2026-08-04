/* hs_real_to_short @0x8368D5E0 — HaloScript type coercion: real script value -> short.
 * DEVIATION: the parameter is `int`, not `float`. The binary opens with `stw r3, arg_14(r1)` /
 * `lfs f0, arg_14(r1)` — the float arrives as a BIT PATTERN in r3, never in f1. Same word-in /
 * word-out typecasting_procedures ABI as hs_long_to_real.
 * DEVIATION: fctiwz truncates float to int32, lower 16 bits packed into MSWord of r3. The int32
 * step is load-bearing, not decoration: it is what makes an out-of-short-range real wrap the way
 * fctiwz+sth does instead of being undefined. */
int hs_real_to_short(int r)
{
    int truncated = (int)*(float *)&r;
    int back_chain;
    *(unsigned short *)&back_chain = (unsigned short)truncated;
    return back_chain;
}
