/* remap_sticks_for_local_player @0x83731CC8 — collapses the alternate-icon stick indices (30/31) onto
 * the base ones (16/17) and swaps left/right when the player's sticks are configured flipped. */

#include <stdint.h>

extern uint8_t should_flip_sticks_for_local_player(int16_t local_player_index);

int16_t remap_sticks_for_local_player(int16_t icon, int16_t local_player_index)
{
    /* Domain is the hud_icon button enum ($9B8B9C52EE51CF6B892253F06A72134A): specific-button range
     * ends at _hud_icon_specific_button_end=17, remapped range _hud_icon_remapped_button_start=18 ..
     * _hud_icon_remapped_button_end=31 (remapped = specific + 14). Here 30/31 (remapped stick icons)
     * collapse onto 16/17 (specific stick icons). No DB enum names the individual 16/17/30/31 slots, so
     * they are left raw. no DB enum found: individual stick-icon slots 16/17/30/31. */
    switch (icon)
    {
    case 16:
    case 30:
        return (should_flip_sticks_for_local_player(local_player_index) != 0) + 16;
    case 17:
    case 31:
        return (should_flip_sticks_for_local_player(local_player_index) == 0) + 16;
    default:
        return icon;
    }
}
