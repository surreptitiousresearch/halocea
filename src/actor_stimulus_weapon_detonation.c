/* actor_stimulus_weapon_detonation @0x837D49B0 — react an AI actor to a weapon detonation near it at `position`.
 * Sibling of actor_stimulus_weapon_impact: it builds the normalized direction from the actor to the detonation
 * (falling back to the actor's own facing when the detonation is essentially on top of it) and, for actors not
 * yet in a high combat state and within the character definition's surprise distance, raises a surprise stimulus
 * (level 4) and records the danger direction, a fresh danger timer (90 ticks), and related danger-state fields.
 * If the detonation came from an enemy object it also raises the actor's persistent "detonation danger" score to
 * at least 900. Finally it drives the actor's secondary look toward the detonation point.
 *
 * The `count` parameter is unused (it is part of the shared stimulus signature). As in the impact sibling, one
 * stack direction_specification is reused two ways — first with a real_vector3d aliased over its first 12 bytes
 * (the surprise direction, for actor_stimulus_surprise), then as a proper direction_specification (type 3,
 * explicit point) for actor_look_secondary. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/direction_specification.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_combat_transition_type.h"
#include "headers/actor_mode.h"
#include "headers/actor_surprise_type.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/blam_data_globals.h"
#include "headers/direction_specification_type.h"


extern void actor_stimulus_surprise(uint16_t actor_index, int16_t surprise_level, int prop_index, const real_vector3d *surprise_vector);
extern void actor_stimulus_combat(int actor_index, int16_t transition_type, const real_point3d *guard_point, int guard_point_surface_index, float guard_distance, int guard_timer, const real_vector3d *transition_vector, int prop_index, int prop_look_timer, uint8_t prop_look_while_moving);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern direction_specification *direction_get_empty(direction_specification *result);
extern uint8_t actor_look_secondary(uint16_t actor_index, int16_t type, int16_t priority, direction_specification *direction);
extern float __fsqrts(double value);
extern double __fabs(double value);


void actor_stimulus_weapon_detonation(int actor_index, int object_index, const real_point3d *position, int16_t count)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *character_definition = TAG_GET(actor_definition, actor->meta.definition_index); /* typed so ->panic resolves */

    direction_specification look_target;
    real_vector3d *surprise = (real_vector3d *)&look_target;

    float dx = position->n[0] - actor->input.position.head_position.x;
    float dz = position->n[2] - actor->input.position.head_position.z;
    float dy = position->n[1] - actor->input.position.head_position.y;
    surprise->n[2] = dz;
    surprise->n[0] = dx;
    surprise->n[1] = dy;

    float length = __fsqrts(dy * dy + (dz * dz + dx * dx));
    if ( __fabs(length) < 0.000099999997 )
    {
        length = 0.0f;
    }
    else
    {
        surprise->n[0] = (1.0f / length) * dx;
        surprise->n[1] = (1.0f / length) * dy;
        surprise->n[2] = (1.0f / length) * dz;
    }

    if ( __fabs(length) < 0.000099999997 )
    {
        /* detonation is at the actor — use the actor's facing vector instead */
        surprise->n[0] = actor->input.facing_vector.n[0];
        surprise->n[1] = actor->input.facing_vector.n[1];
        surprise->n[2] = actor->input.facing_vector.n[2];
    }

    if ( actor->state.mode < _actor_mode_combat && length < character_definition->panic.surprise_distance )
        actor_stimulus_surprise(actor_index, _actor_surprise_unprepared_grenade, -1, surprise);

    /* DEVIATION: the decompiler's transition write block here is a verbatim compiler-inlined copy of
     * actor_stimulus_combat @0x837D3C90 (zero call xrefs — inlined at every caller), constant-folded for
     * guard_point=NULL and the never-null surprise vector — emitted as the call the source had. A prior
     * pass mislabeled the guarded field as stimuli.berserk_type / actor_berserk_damage: disasm
     * 0x837D4AF0/0x837D4B1C reads and stores +0x312 = stimuli.combat_transition, and the constant 3 is
     * _actor_stimulus_combat_impact (actor_berserk_damage is numerically 3 too, hence the confusion).
     * Dead guard args spelled -1/0.0f. */
    actor_stimulus_combat(actor_index, _actor_stimulus_combat_impact, 0, -1, 0.0f, 90,
                          surprise, -1, 0, 0);

    if ( object_index != -1
      && game_team_is_enemy(actor->meta.team_index, ((object_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum)->object.owner_team_index) )
    {
        int danger_level = actor->stimuli.suspicion_combat_status;
        if ( danger_level >= _actor_combat_status_investigate )
        {
            if ( danger_level == _actor_combat_status_investigate )
            {
                int danger_score = actor->stimuli.suspicion_timer;
                if ( danger_score <= 900 )
                    danger_score = 900;
                actor->stimuli.suspicion_timer = danger_score;
            }
        }
        else
        {
            actor->stimuli.suspicion_combat_status = _actor_combat_status_investigate;
            actor->stimuli.suspicion_timer = 900;
        }
    }

    direction_specification empty;
    /* recovered: union-aliased point store (*(int*)&look_target.type copy + prop_index bit-pun over
     * point.n[0]) -> plain empty copy + typed point store, matching the environmental_noise idiom */
    look_target = *direction_get_empty(&empty);
    look_target.type = _direction_specification_point;
    look_target.___u1.point.n[0] = position->n[0];
    look_target.___u1.point.n[1] = position->n[1];
    look_target.___u1.point.n[2] = position->n[2];
    actor_look_secondary(actor_index, _secondary_look_detonation, _secondary_look_priority_default, &look_target);
}
