/* unit_throw_grenade_begin @0x836CFBE0 — start a unit's grenade-throw. Fails if the unit has no
 * grenades of the current type, is already busy in an animation, or holds a weapon that blocks grenade
 * throwing. On success it stops any reload/melee, enters the throw animation state (33), seeds the
 * throw timing from the animation's key frame, aligns the unit's facing (to the supplied vector, or to
 * its current aiming direction), sends the first-person throw message, unzooms, and spawns the grenade
 * throw effect from the unit. Returns 1 when the throw begins, 0 otherwise.
 *
 * DEVIATION: effect_new_from_object's color and impulse_field args are nullptr (disasm li r9,0 / li
 * r10,0); the two 0.0 float scales reserve their GPR slots (FPR-shadow), so the decompiler's v15/v14
 * are phantom. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/game_globals_definition.h"
#include "headers/real_vector2d.h"
#include "headers/unit_animation.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/game_globals_grenade.h"
#include "headers/unit_grenade_throw_state.h"
#include "headers/unit_animation_state.h"
#include "headers/first_person_weapon_message_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern uint8_t unit_animation_busy(unit_animation *animation);
extern uint8_t weapon_prevents_grenade_throwing(int weapon_index);
extern void weapon_stop_reload(int weapon_index);
extern void biped_stop_melee_attack(int biped_index);
extern uint8_t unit_animation_set_state(int unit_index, int16_t new_state);
extern void unit_align_facing(int unit_index, const real_vector2d *alignment_vector);
extern float normalize2d(real_vector2d *v);
extern void first_person_weapon_message_from_unit(int unit_index, int16_t message_type);
extern void player_control_unzoom(int unit_index);
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);

int unit_throw_grenade_begin(int unit_index, const real_vector2d *alignment_vector)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);
    unit_animation *unit_anim = &unit->unit.animation; /* renamed: local shadowed the `animation` typedef used below */

    int16_t weapon_slot = unit->unit.current_weapon_index;
    int weapon_index = -1;
    if (weapon_slot != -1)
        weapon_index = unit->unit.weapon_object_indices[weapon_slot];

    int grenade_type = unit->unit.current_grenade_index;
    int16_t grenade_count = (grenade_type == -1) ? 0 : unit->unit.grenade_counts[grenade_type];

    if (grenade_count <= 0
        || unit_animation_busy(unit_anim)
        || weapon_prevents_grenade_throwing(weapon_index))
        return 0;

    if (weapon_index != -1)
        weapon_stop_reload(weapon_index);
    biped_stop_melee_attack(unit_index);
    unit_anim->action = 0;
    unit_anim->action_animation.index = -1;

    if (!unit_animation_set_state(unit_index, _unit_state_throw_grenade))
        return 0;

    unit->unit.grenade_throw_ticks = 0;
    int16_t animation_index = unit->object.animation.state.index;
    unit->unit.grenade_throw_state = _grenade_throw_state_begin;

    const animation *animations =
        (const animation *)(TAG_GET(animation_graph, definition->object.animation_graph.index))->animations.address;
    unit->unit.grenade_throw_full_power_ticks = animations[animation_index].private_key_frame_index
                               - unit->object.animation.state.frame_index + 1;

    if (alignment_vector)
    {
        unit_align_facing(unit_index, alignment_vector);
    }
    else
    {
        real_vector2d aiming = *(real_vector2d *)&unit->unit.aiming_vector;
        if (normalize2d(&aiming) > 0.0f)
            unit_align_facing(unit_index, &aiming);
    }

    first_person_weapon_message_from_unit(unit_index, _first_person_weapon_message_throw_grenade);
    player_control_unzoom(unit_index);

    int throw_effect = ((const game_globals_grenade *)global_game_globals->grenades.address)[unit->unit.current_grenade_index].throwing_effect.index;
    if (throw_effect != -1)
        effect_new_from_object(throw_effect, unit_index, unit_index, -1, 0.0f, 0.0f, nullptr, nullptr);

    return 1;
}
