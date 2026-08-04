#include <stdint.h>
/* encode_grenade_index — pack a grenade selection into the network-compacted wire form (inverse of
 * decode_grenade_index). grenade_index == -1 (NONE) sets bit 1 (the "no grenade" sentinel) via
 * cntlzw(grenade_index+1)==32; a valid index leaves bit 1 clear and stores the low index bit in bit 0,
 * matching the unit_grenade_type enum (DB $C04139DE66CD4C6C6A695EB26A7024EE:
 * 0 = _unit_grenade_human_fragmentation, 1 = _unit_grenade_covenant_plasma). Wire-local bit layout, raw. */

/* DEVIATION: PPC cntlzw — see encode_weapon_index.c for portable implementation note. */
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

unsigned int encode_grenade_index(int16_t grenade_index)
{
    return (ppc_cntlzw((unsigned int)(grenade_index + 1)) >> 4) & 2 | grenade_index & 1;
}
