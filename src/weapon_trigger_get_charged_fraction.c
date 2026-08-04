/* weapon_trigger_get_charged_fraction @0x836D9830 — how charged one weapon trigger is, in [0,1]: 0 unless
 * the trigger is charging or charged. While charging (state byte 2) the fraction is
 * 1 - (ticks_remaining/30) / definition charging_time (trigger definition float +72, 276-byte trigger
 * definition stride at weapon definition dword[320]); a fully charged trigger (state 3) returns 1.
 * Trigger runtime records are 40 bytes: state byte at weapon+40*trigger+609, tick timer word at +610.
 *
 * DEVIATION (bug class 9): the decompiler merged the two int params into one __int64 register pun and
 * returned via a double-halves pun; rendered with the DB's 2-arg prototype and a plain float return. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/weapon_trigger_state.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


float weapon_trigger_get_charged_fraction(int weapon_index, int16_t trigger_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_trigger *trigger = &weapon->weapon.triggers[trigger_index];

    int trigger_state = (unsigned __int8)trigger->state;
    if ( trigger_state == _weapon_trigger_charged )
        return 1.0f;
    if ( trigger_state != _weapon_trigger_charging )
        return 0.0f;

    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    float charging_time =
        ((weapon_trigger_definition *)definition->weapon.triggers.address)[trigger_index].charging_time;
    __int16 ticks_remaining = trigger->state_timer;
    return 1.0f - ((float)ticks_remaining * SECONDS_PER_TICK) / charging_time;
}
