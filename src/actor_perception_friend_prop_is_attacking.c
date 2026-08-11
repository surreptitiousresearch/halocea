/* actor_perception_friend_prop_is_attacking @0x837D7C10 — decide whether a friendly prop (a unit this
 * actor is aware of) is currently attacking, and fill attack_vector with the attack direction. Swarm props
 * never count. For a player-controlled friend, the attack vector is its aiming vector; if the friend is
 * already flagged as shooting that is reported directly, otherwise — when the actor is itself engaged — the
 * actor's own perceived enemies are scanned for one lying within ~60 degrees of the aim (returns 1 if
 * found). For a non-player friend, defers to actor_attacking_target on the friend's actor.
 * Returns nonzero when the friend is judged to be attacking. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/real_vector3d.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern void unit_get_aiming_vector(int unit_index, real_vector3d *aiming_vector);
extern int actor_attacking_target(int actor_index, real_vector3d *attack_vector);
extern void prop_iterator_new(prop_iterator *iterator, int actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern float normalize3d(real_vector3d *v);

uint8_t actor_perception_friend_prop_is_attacking(int actor_index, int friend_prop_index,
                                                  real_vector3d *attack_vector)
{
    prop_datum *friend_prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, friend_prop_index);
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if (friend_prop->swarm)
        return 0;

    if (!friend_prop->player)
    {
        int friend_actor_index = friend_prop->actor_index;
        if (friend_actor_index == -1)
            return 0;
        return actor_attacking_target(friend_actor_index, attack_vector);
    }

    int shooting = friend_prop->shooting;
    unit_get_aiming_vector(friend_prop->unit_index, attack_vector);
    if (shooting)
        return shooting;
    if (actor->situation.known_enemies <= 0) /* actor not engaged enough to corroborate */
        return shooting;

    prop_iterator iterator;
    prop_iterator_new(&iterator, actor_index);
    for (prop_datum *prop = prop_iterator_next(&iterator); prop; prop = prop_iterator_next(&iterator))
    {
        if (prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged && prop->enemy)
        {
            real_vector3d to_enemy;
            to_enemy.n[0] = prop->body_position.n[0] - friend_prop->body_position.n[0];
            to_enemy.n[1] = prop->body_position.n[1] - friend_prop->body_position.n[1];
            to_enemy.n[2] = prop->body_position.n[2] - friend_prop->body_position.n[2];
            if (normalize3d(&to_enemy) > 0.0
                && attack_vector->n[0] * to_enemy.n[0]
                       + (attack_vector->n[2] * to_enemy.n[2] + attack_vector->n[1] * to_enemy.n[1]) > 0.5)
                return 1;
        }
    }

    return shooting;
}
