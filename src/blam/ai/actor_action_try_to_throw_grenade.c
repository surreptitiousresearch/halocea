/* actor_action_try_to_throw_grenade @0x837F3030 — attempt to actually execute a previously-planned
 * grenade throw. Fails if the actor's unit is busy or currently flinching from body damage. Unless the
 * trajectory was already known-good (known_trajectory), re-validates the throw is still viable
 * (actor_action_test_grenade); if not, clears the want-to-throw flag. Bails if the want-to-throw flag
 * isn't set. Requires the actor to still be facing roughly toward its planned grenade target (within
 * ~30 degrees, cos >= 0.866) — computed from the target position (established grenade-plan fields,
 * actor_combat_plan_grenade_trajectory.c) relative to the actor's body position, compared against its
 * facing vector. On success, clears the want-to-throw flag, sets the "committed to throw" flag (+1116),
 * and (if part of an encounter) stamps the encounter's last-grenade-thrown time. Returns whether the
 * throw was committed.
 *
 * Resolved: the "committed to throw" flag is actor->orders.combat.throw_grenade (+1116), and the
 * encounter stamp is encounter_datum.last_grenade_throw_time (+92) — both DB-named now. +236 (object's
 * current_body_damage, via the established object_datum "+4" convention), +1704/+1708 (grenade target,
 * established in actor_combat_plan_grenade_trajectory.c), +300/+304 (input.position.body_position,
 * established in action_flee_at_flee_position.c) and +372/+376 (input.facing_vector) all reuse
 * already-established fields. */

#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_vector2d.h"
#include "headers/blam_data_globals.h"


extern uint8_t unit_is_busy(int object_index);
extern uint8_t actor_action_test_grenade(int actor_index);
extern float normalize2d(real_vector2d *v);
extern int game_time_get(void);

int actor_action_try_to_throw_grenade(int actor_index, uint8_t known_trajectory)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    int unit_index = actor->meta.unit_index;
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    if ( unit_is_busy(unit_index) || unit->object.current_body_damage > 0.0f )
        return 0;

    if ( !known_trajectory && !actor_action_test_grenade(actor_index) )
        actor->control.grenade_trying_to_throw = 0;

    if ( !actor->control.grenade_trying_to_throw )
        return 0;

    real_vector2d to_target;
    to_target.n[0] = actor->control.grenade_current_target.x - actor->input.position.body_position.n[0];
    to_target.n[1] = actor->control.grenade_current_target.y - actor->input.position.body_position.n[1];

    if ( normalize2d(&to_target) <= 0.0f
      || actor->input.facing_vector.n[1] * to_target.n[1] + to_target.n[0] * actor->input.facing_vector.n[0] < 0.86602539f )
        return 0;

    int encounter_index = actor->meta.encounter_index;
    actor->control.grenade_trying_to_throw = 0;
    actor->orders.combat.throw_grenade = 1;

    if ( encounter_index != -1 )
    {
        encounter_datum *encounter = DATUM_GET(encounter_data, encounter_datum, encounter_index);
        encounter->last_grenade_throw_time = game_time_get();
    }

    return 1;
}
