/* actor_discard_firing_position @0x837EFD68 — record a firing position the actor is abandoning so it
 * won't immediately re-pick it. Pushes (temporary flag, firing_position_index) into the actor's 4-entry
 * recently-discarded ring buffer (actor +968, stride 4, index word +966 advanced mod 4), then stashes the
 * discarded position's world point (firing position +0, 3 floats, from the actor's encounter firing-
 * position list) into the actor's last-discarded-position fields (+988) with the temporary flag.
 * Raw actor-datum offsets (stride 1828) reproduced from the decompiler. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/firing_position_definition.h"
#include "headers/blam_data_globals.h"


void actor_discard_firing_position(uint16_t actor_index, int16_t firing_position_index, uint8_t temporary)
{
    if ( firing_position_index != -1 )
    {
        actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
        int ring_index = actor->firing_positions.next_discarded_firing_positions_entry;
        actor->firing_positions.discarded_firing_positions[ring_index].temporary = temporary;
        actor->firing_positions.discarded_firing_positions[ring_index].index = firing_position_index;

        encounter_definition *encounter =
            &((encounter_definition *)global_scenario->ai_encounters.address)[(unsigned __int16)actor->meta.encounter_index];
        actor->firing_positions.next_discarded_firing_positions_entry =
            (actor->firing_positions.next_discarded_firing_positions_entry + 1) % 4;

        /* 24-byte firing_position_definition; position (real_point3d) is the world point. */
        firing_position_definition *firing_position =
            &((firing_position_definition *)encounter->firing_positions.address)[firing_position_index];
        actor->firing_positions.last_discarded_firing_position_valid = 1;
        actor->firing_positions.last_discarded_firing_position_temporary = temporary;
        actor->firing_positions.last_discarded_firing_position.x = firing_position->position.x;
        actor->firing_positions.last_discarded_firing_position.y = firing_position->position.y;
        actor->firing_positions.last_discarded_firing_position.z = firing_position->position.z;
    }
}
