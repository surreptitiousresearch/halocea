#include "dsSTR_HASH.h"

/* CAVEAT: the decompiler flagged "local variable allocation has failed" here and packed the
 * 64-bit hash accumulator and the string cursor into OVERLAPPED register halves. The body below
 * is reconstructed from the disassembly (0x82527F58): a single 64-bit accumulator mixed with
 * signed 64-bit multiplies (mulld) and left shifts (sld), finalized with a magic-number reduction
 * (mulhdu by 0xBFFFFFE5C0000397, >>30, then subtract q*0x55555561) that computes hash % 0x55555561.
 * `mul`/`add` are sign-extended to 64 bits before use, matching extsw r7,r4 / extsw r6,r5. */

// dsSTR_HASH::hash @ 0x82527F58
// Hash the NUL-terminated `str`: fold each 4-byte word (then each trailing byte) as
// (chunk*mul + add) << rolling_shift into a 64-bit accumulator via XOR, then reduce modulo
// 0x55555561 (1431655777) and return the 32-bit remainder.
int dsSTR_HASH::hash(const char *str, unsigned int mul, unsigned int add)
{
    long long multiplier = (int)mul; // extsw
    long long adder = (int)add;      // extsw
    unsigned long long accumulator = 0;
    int shift = 0; // rolling 0..7 left-shift count

    const char *cursor = str;
    while (*cursor)
        ++cursor;
    int length = (int)(cursor - str);

    int wordCount = length >> 2;
    const int *words = (const int *)str; // signed 32-bit word view (extsw of each lwzu)
    for (int i = 0; i < wordCount; ++i) {
        long long term = ((long long)words[i] * multiplier + adder) << shift;
        accumulator ^= (unsigned long long)term;
        shift = (shift + 1) & 7;
    }

    for (int i = wordCount * 4; i < length; ++i) {
        long long term = ((long long)(unsigned char)str[i] * multiplier + adder) << shift;
        accumulator ^= (unsigned long long)term;
        shift = (shift + 1) & 7;
    }

    return (int)(accumulator % 0x55555561ull);
}
