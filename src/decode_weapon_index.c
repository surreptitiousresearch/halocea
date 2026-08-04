/* decode_weapon_index — unpack the 3-bit compacted weapon-index wire form: bit 2 is the "no weapon"
 * (NONE) sentinel -> -1; bits 0-1 are the 2-bit weapon slot index [0,3]. Structural wire layout (matches
 * default_fixed_width_weapon_index_decoder.c); bit widths are protocol constants, not a DB enum. */
int decode_weapon_index(char compacted_form)
{
    if (compacted_form & 4)
        return -1;
    return compacted_form & 3;
}
