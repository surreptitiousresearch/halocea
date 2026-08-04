/* actor_derive_target_information @0x8371DF3C — copy a fighting friend's current target to this actor by
 * creating an "orphan" perception of the unit the friend's target prop refers to. No-op if the friend has
 * no current target prop. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"


extern int actor_perception_create_orphan_from_friend(int actor_index, int unit_index, int friend_actor_index, int friend_prop_index);

void actor_derive_target_information(int actor_index, uint16_t friend_actor_index)
{
    actor_datum *friend_actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, friend_actor_index);
    int prop_index = friend_actor->target.target_prop_index;
    if (prop_index != -1)
        actor_perception_create_orphan_from_friend(
            actor_index,
            (DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index))->unit_index,
            friend_actor_index,
            prop_index);
}
