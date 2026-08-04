/* pin_aiming_for_unit @0x836C6C70 — decide whether the camera should pin (snap) its aim to a unit.
 * Pinning is allowed by default. During an active game engine, a player-controlled unit
 * (unit.player_index != -1) that is NOT seated in a vehicle — i.e. it has no parent object
 * (object.parent_object_index == -1) or an invalid seat (unit.parent_seat_index == NONE) — should
 * not be pinned, so returns false. Otherwise returns true. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_engine_running(void);

uint8_t pin_aiming_for_unit(int unit_index)
{
    if (game_engine_running() && unit_index != -1)
    {
        unit_datum *unit = (unit_datum *)
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

        if (unit->unit.player_index != -1
            && (unit->object.parent_object_index == -1
                || (unsigned __int16)unit->unit.parent_seat_index == 0xFFFF))
            return 0;
    }
    return 1;
}
