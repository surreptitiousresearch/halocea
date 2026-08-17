/* encode_weapon_index @0x8379D380 */
#include <stdint.h>
/* encode_weapon_index — pack a weapon-slot selection into the 3-bit compacted wire form (inverse of
 * decode_weapon_index). weapon_index == -1 (NONE) sets bit 2 (the "no weapon" sentinel) via
 * cntlzw(weapon_index+1)==32; a valid slot leaves bit 2 clear and stores the 2-bit slot index [0,3] in
 * bits 0-1. Structural wire layout; bit widths are protocol constants, not a DB enum. */

/* DEVIATION: PPC cntlzw (count leading zeros) has no standard C equivalent.
 * Rendered as __builtin_clz where available; portable form shown here. */
static unsigned int ppc_cntlzw(unsigned int x)
{
    if (x == 0) return 32;
    unsigned int n = 0;
    if ((x & 0xFFFF0000u) == 0) { n += 16; x <<= 16; }
    if ((x & 0xFF000000u) == 0) { n +=  8; x <<=  8; }
    if ((x & 0xF0000000u) == 0) { n +=  4; x <<=  4; }
    if ((x & 0xC0000000u) == 0) { n +=  2; x <<=  2; }
    if ((x & 0x80000000u) == 0) { n +=  1; }
    return n;
}

unsigned int encode_weapon_index(int16_t weapon_index)
{
    return (ppc_cntlzw((unsigned int)(weapon_index + 1)) >> 3) & 4 | weapon_index & 3;
}
