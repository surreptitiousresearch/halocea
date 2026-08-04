/* reply_filter_same_platoon @0x837CC118 — AI dialogue reply filter: accept a candidate reply actor only
 * when the base proximity filter (reply_filter_close) passes, the original unit is actor-controlled
 * (unit+500 = its actor index), and both actors share the same encounter (actor dword +52) and the same
 * platoon (actor word +60). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/ai_information_packet.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


extern uint8_t reply_filter_close(int original_unit_index, ai_information_packet *communication, int reply_actor_index);

uint8_t reply_filter_same_platoon(int original_unit_index, ai_information_packet *communication, int reply_actor_index)
{
    if ( !reply_filter_close(original_unit_index, communication, reply_actor_index) )
        return 0;

    unit_datum *original_unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, original_unit_index)->datum;
    int original_actor_index = original_unit->unit.actor_index;
    if ( original_actor_index == -1 || reply_actor_index == -1 )
        return 0;

    actor_datum *original_actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, original_actor_index);
    actor_datum *reply_actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, reply_actor_index);

    int encounter_index = original_actor->meta.encounter_index;
    if ( encounter_index == -1 )
        return 0;
    if ( encounter_index != reply_actor->meta.encounter_index )
        return 0;
    return original_actor->meta.platoon_index == reply_actor->meta.platoon_index;
}
