/* ai_handle_damage @0x836E8F08 — notify the AI systems that a unit took (non-fatal) damage. Resolves the
 * unit responsible for the damaging object (driver/operator via actor fields [201]/[202], unless the damage
 * category is _damage_category_vehicle), forwards the hit to the victim's actor, raises an AI communication event whose subtype
 * encodes the relationship (self/ally/enemy) and severity, and provokes allegiance between the two teams.
 *
 * ABI verified from prologue (0x836E8F08) and call site (0x836D5E84): 6 args, fraction is a single-precision
 * float reserving r6. The decompiler's `delayed` param is a phantom from that reserved slot; its real
 * `information_data` (= r8) is the delayed flag, used here as `delayed`. `damage_category` is an __int16 (the decompiler
 * mistypes it as real_vector3d*); actor_handle_damage's 4th arg is the damage velocity (r6), not the
 * category.
 *
 * Deviation: the enemy test is rendered as a _cntlzw bit-twiddle — it is game_team_is_enemy(...) ? 3 : 2. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_vector3d.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/object_type.h"
#include "headers/damage_category.h"
#include "headers/blam_data_globals.h"

#include "headers/ai_information_data.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void actor_handle_damage(int actor_index, int aggressor_unit_index, float damage_fraction, real_vector3d *damage_velocity);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern void game_allegiance_provoke(int16_t aggressor_team_index, int16_t victim_team_index);

void ai_handle_damage(int unit_index, int shooter_object_index, int16_t damage_category,
                      float fraction, real_vector3d *damage_velocity, uint8_t delayed)
{
    int responsible_unit_index = -1;
    unit_datum *victim_object_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    unit_datum *shooter_unit;
    unit_datum *responsible_object_data;
    if ( shooter_object_index == -1
      || (shooter_unit = object_try_and_get_and_verify_type(shooter_object_index, object_mask_unit)) == 0
      || (damage_category == _damage_category_vehicle || (responsible_unit_index = shooter_unit->unit.gunner_object_index, responsible_unit_index == -1))
         && (responsible_unit_index = shooter_unit->unit.driver_object_index, responsible_unit_index == -1)
         && (responsible_unit_index = shooter_object_index, shooter_object_index == -1) )
    {
        responsible_object_data = 0;
    }
    else
    {
        responsible_object_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, responsible_unit_index)->datum);
    }

    if ( !delayed && damage_category != _damage_category_falling )
    {
        int actor_index = victim_object_data->unit.actor_index;
        if ( actor_index != -1 )
            actor_handle_damage(actor_index, responsible_unit_index, fraction, damage_velocity);
    }

    int16_t event_subtype = _comm_hostility_none;
    if ( unit_index == responsible_unit_index )
    {
        event_subtype = _comm_hostility_self;
    }
    else if ( responsible_object_data )
    {
        event_subtype = game_team_is_enemy(victim_object_data->object.owner_team_index,
                                           responsible_object_data->object.owner_team_index) ? _comm_hostility_enemy : _comm_hostility_friend;
    }

    int16_t event_type;
    if ( delayed || event_subtype != _comm_hostility_friend )
    {
        if ( fraction < 0.30000001f )
            goto provoke;  /* minor enemy/self damage: no comm event */
        event_type = _ai_communication_hurt;
    }
    else
    {
        event_subtype = _comm_hostility_friend;  /* ally damage */
        event_type = _ai_communication_damage;
    }
    ai_communication_event(event_type, unit_index, responsible_unit_index, event_subtype, damage_category,
                           -1, 0);

provoke:
    if ( responsible_object_data )
        game_allegiance_provoke(responsible_object_data->object.owner_team_index,
                                victim_object_data->object.owner_team_index);
}
