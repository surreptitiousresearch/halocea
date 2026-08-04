/* valid_realcmp @0x8370F608 — true when two reals are equal within 0.001 and their difference is not NaN.
 * Used by the orthonormal-basis validators. */

#include <stdint.h>

extern double __fabs(double x);
extern int _isnan(double x);

uint8_t valid_realcmp(float x, float y)
{
    float difference = x - y;
    /* disasm 8370F61C: fmr f1, f31 (the fsubs result) before bl _isnan — the binary tests the
     * DIFFERENCE for NaN, not y (previous source was wrong). */
    if (_isnan(difference))
        return 0;
    /* epsilon is the float constant 0x3A83126F (0.001f), disasm 8370F634 lfs — keep the f suffix
     * so the promoted double matches the binary's comparison exactly */
    if (__fabs(difference) >= 0.001f)
        return 0;
    return 1;
}
