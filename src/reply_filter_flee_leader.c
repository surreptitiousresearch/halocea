/* reply_filter_flee_leader @0x837CC380 — true if the replying actor is fighting and its meta.type is 0.
 * (disasm: stride 0x724 = sizeof(actor_datum), loads halfword at byte +4 = actor->meta.type.) */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/ai_information_packet.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t actor_is_fighting(uint16_t actor_index);

uint8_t reply_filter_flee_leader(int original_unit_index, ai_information_packet *communication, int reply_actor_index)
{
    if ( !actor_is_fighting(reply_actor_index) )
        return 0;

    actor_datum *actor = DATUM_GET(actor_data, actor_datum, reply_actor_index);
    if ( actor->meta.type )
        return 0;

    return 1;
}
