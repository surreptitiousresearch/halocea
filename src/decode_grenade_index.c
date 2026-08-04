/* decode_grenade_index — unpack a network-compacted grenade selection. Bit 1 is the "no grenade"
 * sentinel (returns -1 / NONE); bit 0 is the grenade type index, matching the unit_grenade_type enum
 * (DB $C04139DE66CD4C6C6A695EB26A7024EE: 0 = _unit_grenade_human_fragmentation,
 * 1 = _unit_grenade_covenant_plasma). Wire-local bit layout, left raw. */
int decode_grenade_index(char compacted_form)
{
    if (compacted_form & 2)
        return -1;
    return compacted_form & 1;
}
