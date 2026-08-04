/* unit_definition_get_active_hud_index @0x837BC5C4 — pick which of a unit's HUD references is active and
 * return that HUD's globals tag index. Single-player (and split-screen co-op) use HUD slot 0; other
 * multiplayer uses slot 1. The slot is clamped to the last available reference. Returns -1 when the unit
 * defines no HUDs (or the clamped slot is negative). */

#include <stdint.h>
#include "headers/unit_definition.h"
#include "headers/unit_hud_reference.h"
#include "headers/blam_data_globals.h"


int unit_definition_get_active_hud_index(const unit_definition *unit_definition, uint8_t in_multiplayer)
{
    __int16 hud_slot;
    if (!in_multiplayer || hcex_coop_local_player_index >= 0)
        hud_slot = 0;
    else
        hud_slot = 1;

    int slot = unit_definition->unit.huds.count - 1;
    if (hud_slot <= slot)
        slot = hud_slot;

    if ((__int16)slot < 0)   /* no HUD slot available (huds.count was 0 -> slot -1); &0x8000 is the int16 sign bit */
        return -1;
    return ((unit_hud_reference *)unit_definition->unit.huds.address)[(__int16)slot].hud.index;
}
