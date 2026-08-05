/* unit_definition_get_seat_active_hud_index @0x837BC618 — resolve a seat's currently-active HUD tag index:
 * clamp a coop-mode preference (1 if multiplayer and not the coop host, else 0) against the seat's
 * seat_huds.count-1, then index seat->seat_huds for the selected unit_hud_reference's .hud.index.
 * -1 if the clamped selector is negative. */

#include <stdint.h>
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_hud_reference.h"
#include "headers/blam_data_globals.h"


int unit_definition_get_seat_active_hud_index(const unit_definition *unit_definition, int16_t seat_index, uint8_t in_multiplayer)
{
    const unit_seat *seat = &((const unit_seat *)unit_definition->unit.seats.address)[seat_index];
    int16_t preferred_hud = (in_multiplayer && hcex_coop_local_player_index < 0) ? 1 : 0;
    int16_t hud_count_minus_one = seat->seat_huds.count - 1;
    int16_t active_hud_index = (preferred_hud <= hud_count_minus_one) ? preferred_hud : hud_count_minus_one;

    if ( active_hud_index < 0 )
        return -1;

    return ((unit_hud_reference *)seat->seat_huds.address)[active_hud_index].hud.index;
}
