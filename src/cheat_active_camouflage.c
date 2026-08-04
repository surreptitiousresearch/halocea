/* cheat_active_camouflage @0x8369A6D8 — give the acting player's unit full active camouflage. Sets the
 * unit's camo intensity (unit object +0x37C) to 1.0 and raises the active-camo flags (+0x204): if the
 * active-camouflaged bit was already set, also set the super-camouflaged bit, then ensure active-camouflaged. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_flags.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern int cheat_player_index(void);

int cheat_active_camouflage(void)
{
    int player_index = cheat_player_index();
    if ( player_index == -1 )
        return -1;

    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int flags = unit_object->unit.flags;
    unit_object->unit.active_camouflage = 1.0f;
    int already_active = flags & (1u << _unit_active_camouflaged_bit);
    if ( already_active )
        unit_object->unit.flags = flags | (1u << _unit_super_camouflaged_bit);
    unit_object->unit.flags |= (1u << _unit_active_camouflaged_bit);
    return already_active;
}
