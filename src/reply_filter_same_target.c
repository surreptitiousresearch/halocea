/* reply_filter_same_target @0x837CC250 — reply filter: passes reply_filter_close, then requires the
 * original unit to have a controlling actor (unit +500, dword) and a valid reply actor, and that both the
 * controlling actor's and the reply actor's own prop-of-interest (actor +624 -> prop index, the same field
 * actor_target_unit_index.c reads) resolve to valid props sharing the same target unit_index. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/ai_information_packet.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"


extern uint8_t reply_filter_close(int original_unit_index, ai_information_packet *communication, int reply_actor_index);

int reply_filter_same_target(int original_unit_index, ai_information_packet *communication, int reply_actor_index)
{
    if ( !reply_filter_close(original_unit_index, communication, reply_actor_index) )
        return 0;

    unit_datum *original_unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, original_unit_index)->datum;
    int controlling_actor_index = original_unit->unit.actor_index;
    if ( controlling_actor_index == -1 )
        return 0;
    if ( reply_actor_index == -1 )
        return 0;

    actor_datum *reply_actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, reply_actor_index);
    int reply_prop_index = reply_actor->target.target_prop_index;

    actor_datum *controlling_actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, controlling_actor_index);
    int original_prop_index = controlling_actor->target.target_prop_index;

    if ( original_prop_index == -1 )
        return 0;
    if ( reply_prop_index == -1 )
        return 0;

    prop_datum *original_prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, original_prop_index);
    prop_datum *reply_prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, reply_prop_index);
    return original_prop->unit_index == reply_prop->unit_index;
}
