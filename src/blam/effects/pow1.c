/* pow1 @0x8380ACE8 — z^e, special-cased to skip the pow() call when e==1.0.
 *
 * DEVIATION: the decompiler's local-variable allocation failed on this function and it invented a bogus
 * 3rd `long double a3` parameter; DB prototype and disasm confirm a plain 2-param `float pow1(float,float)`. */

extern double pow(double x, double y);

float pow1(float z, float e)
{
    if (e == 1.0f)
        return z;

    return (float)pow(z, e);
}
