/* actor_stimulus_weapon_impact @0x837D4768 — react an AI actor to a weapon impact near it at `position`. If
 * the actor is already tracking this object as a danger source it fires a communication event; otherwise it
 * builds the (normalized) direction from the actor to the impact (falling back to the actor's own facing when
 * the impact is essentially at the actor), and — for actors not yet in a high combat state and within the
 * character definition's surprise distance — raises a surprise stimulus and records the danger direction, a
 * fresh danger timer (90 ticks), and related danger-state fields. Finally it drives the actor's secondary
 * look toward the impact point.
 *
 * DEVIATION/quirk: the decompiler reuses one stack slot for two purposes and register-puns it. The first use
 * overlays a real_vector3d on the direction_specification's first 12 bytes (the surprise direction), passed to
 * actor_stimulus_surprise; the second use is a proper direction_specification (type 3 = explicit point). Both
 * are reproduced via an aliased vector pointer over a single direction_specification local. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/actor_combat_transition_type.h"
#include "headers/actor_mode.h"
#include "headers/ai_communication_type.h"
#include "headers/actor_surprise_type.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_data.h"
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern void actor_stimulus_surprise(uint16_t actor_index, int16_t surprise_level, int prop_index, const real_vector3d *surprise_vector);
extern void actor_stimulus_combat(int actor_index, int16_t transition_type, const real_point3d *guard_point, int guard_point_surface_index, float guard_distance, int guard_timer, const real_vector3d *transition_vector, int prop_index, int prop_look_timer, uint8_t prop_look_while_moving);
extern direction_specification *direction_get_empty(direction_specification *result);
extern uint8_t actor_look_secondary(uint16_t actor_index, int16_t type, int16_t priority, direction_specification *direction);
extern float __fsqrts(double value);
extern double __fabs(double value);

void actor_stimulus_weapon_impact(int actor_index, int object_index, const real_point3d *position, int16_t count)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *character_definition = TAG_GET(actor_definition, actor->meta.definition_index); /* typed so ->panic resolves */

    direction_specification look_target;
    real_vector3d *surprise = (real_vector3d *)&look_target;

    if ( actor->danger_zone.danger_type > actor_danger_zone_none
      && actor->danger_zone.object_index == object_index
      && actor->danger_zone.acknowledgement_timer > 0 )
    {
        ai_communication_event(_ai_communication_grenade_startle, actor->meta.unit_index, -1, -1, -1, -1, nullptr);
    }
    else
    {
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
            /* impact is at the actor — use the actor's facing vector instead */
            surprise->n[0] = actor->input.facing_vector.n[0];
            surprise->n[1] = actor->input.facing_vector.n[1];
            surprise->n[2] = actor->input.facing_vector.n[2];
        }

        if ( actor->state.mode < _actor_mode_combat && length < character_definition->panic.surprise_distance )
            actor_stimulus_surprise(actor_index, _actor_surprise_unprepared_weapon_impact_close, -1, surprise);

        /* DEVIATION: the decompiler's transition write block here is a verbatim compiler-inlined copy
         * of actor_stimulus_combat @0x837D3C90 (zero call xrefs — inlined at every caller),
         * constant-folded for guard_point=NULL and the never-null surprise vector — emitted as the call
         * the source had. A prior pass mislabeled the guarded field as stimuli.berserk_type /
         * actor_berserk_damage: disasm 0x837D48F4/0x837D4920 reads and stores +0x312 =
         * stimuli.combat_transition, and the constant 3 is _actor_stimulus_combat_impact
         * (actor_berserk_damage is numerically 3 too, hence the confusion). Dead guard args spelled
         * -1/0.0f. */
        actor_stimulus_combat(actor_index, _actor_stimulus_combat_impact, 0, -1, 0.0f, 90,
                              surprise, -1, 0, 0);
    }

    direction_specification empty;
    /* recovered: union-aliased point store (*(int*)&look_target.type copy + prop_index bit-pun over
     * point.n[0]) -> plain empty copy + typed point store, matching the environmental_noise idiom */
    look_target = *direction_get_empty(&empty);
    look_target.type = _direction_specification_point;
    look_target.___u1.point.n[0] = position->n[0];
    look_target.___u1.point.n[1] = position->n[1];
    look_target.___u1.point.n[2] = position->n[2];
    actor_look_secondary(actor_index, _secondary_look_weapon_impact, _secondary_look_priority_default, &look_target);
}
