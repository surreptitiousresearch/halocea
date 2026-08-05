/* actor_stimulus_noticed_danger_zone @0x837D5128 — react to noticing a dangerous area (e.g. a grenade or
 * hazard). For a projectile danger (actor_danger_zone_projectile) the actor broadcasts a danger AI-communication scaled by
 * hostility, and if close enough and not already in combat it triggers a surprise. In all cases a
 * not-yet-in-combat actor with a low-priority current stimulus latches a "flee the danger" stimulus
 * pointing away from the danger position.
 *
 * The actor datum and its variant definition have no reversed structs; fields are accessed by database
 * byte offset. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/actor_mode.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/actor_danger_zone_hostility.h"
#include "headers/actor_combat_transition_type.h"
#include "headers/actor_surprise_type.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_data.h"
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern float normalize3d(real_vector3d *v);
extern void actor_stimulus_surprise(uint16_t actor_index, int16_t surprise_level, int prop_index, const real_vector3d *surprise_vector);
extern void actor_stimulus_combat(int actor_index, int16_t transition_type, const real_point3d *guard_point, int guard_point_surface_index, float guard_distance, int guard_timer, const real_vector3d *transition_vector, int prop_index, int prop_look_timer, uint8_t prop_look_while_moving);

void actor_stimulus_noticed_danger_zone(uint16_t actor_index, int16_t danger_type, int16_t danger_hostility,
                                        int danger_object_index, const real_point3d *position)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int unit_index = actor->meta.unit_index;
    actor_definition *variant = TAG_GET(actor_definition, actor->meta.definition_index); /* typed so ->panic resolves */
    if (unit_index == -1)
        return;

    if (danger_type == actor_danger_zone_projectile)
    {
        int16_t hostility = -1;
        if (danger_hostility)   /* != _actor_danger_hostility_enemy */
        {
            if (danger_hostility == _actor_danger_hostility_friend)
                hostility = _comm_hostility_friend;
            else if ((unsigned int)danger_hostility < NUMBER_OF_ACTOR_DANGER_ZONE_HOSTILITIES)
                hostility = _comm_hostility_self;   /* _actor_danger_hostility_self */
        }
        else
        {
            hostility = _comm_hostility_enemy;
        }
        ai_communication_event(_ai_communication_grenade_sighted, unit_index, -1, hostility, -1, -1, nullptr);
    }

    real_vector3d to_actor;
    to_actor.n[0] = position->n[0] - actor->input.position.head_position.n[0];
    to_actor.n[1] = position->n[1] - actor->input.position.head_position.n[1];
    to_actor.n[2] = position->n[2] - actor->input.position.head_position.n[2];
    float distance = normalize3d(&to_actor);

    if (actor->state.mode < _actor_mode_combat && distance < variant->panic.surprise_distance && danger_type == actor_danger_zone_projectile)
        actor_stimulus_surprise(actor_index, _actor_surprise_unprepared_grenade, -1, &to_actor);

    /* DEVIATION: the decompiler's combat-transition write block here is a verbatim compiler-inlined
     * copy of actor_stimulus_combat @0x837D3C90 (zero call xrefs — inlined at every caller),
     * constant-folded for guard_point=NULL and the never-null &to_actor vector — emitted as the call
     * the source had. Dead guard args spelled -1/0.0f. */
    actor_stimulus_combat(actor_index, _actor_stimulus_combat_danger, 0, -1, 0.0f, 0,
                          &to_actor, -1, 0, 0);
}
