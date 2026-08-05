/* unit_ping_animation @0x836D1B28 — play a unit's reaction to being hit: a "ping" flinch, a hard stagger,
 * or a death animation, plus an optional facing realignment toward the hit. Decides, from the damage just
 * taken (object.current_body_damage/current_shield_damage vs the unit definition's flinch/hard-ping/death
 * thresholds) and the caller's flags, whether to (a) overlay a soft ping animation, or (b) drive a full
 * hard-ping/death animation state (23 = ping, 24 = special death, 25 = death). On a successful hard/death
 * animation it also seeds the post-death "twitch" timer (unit.weapon_drop_delay_ticks — DB name kept
 * verbatim) and, when an alignment vector is supplied, rotates the unit to face the hit direction
 * (bucketed by `angle` into front/left/right/back).
 *
 * ABI: verified from the call site (0x836D5D24) and prologue. The decompiler explodes the tail into ~20
 * phantom integer params from float-skip and outgoing-arg slots. The real argument list (register order) is:
 *   r3 unit_index, r4 died, r5 feign_death, r6 died_flying, r7 ignore_hard_pings, r8 force_hard_pings,
 *   r9 alignment_vector_unused (the caller passes &vec here but the body ignores it),
 *   r10 body_part (__int16), f1 angle, stack0 alignment_vector (nullable), stack1 gate.
 * The decompiler's `damage_part` is r9 (unused), its `alignment_vector` is really r10 (body_part, used as an
 * int16), its `a29` is the real nullable alignment vector (stack0), and its `a31` is the gate (stack1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/real_vector2d.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/unit_animation.h"
#include "headers/game_connection.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_flags.h"
#include "headers/biped_datum_flags.h"
#include "headers/biped_definition_flags.h"
#include "headers/unit_definition_flags.h"
#include "headers/object_type.h"
#include "headers/unit_animation_state.h"
#include "headers/unit_animation_flags.h"
#include "headers/base_seat.h"
#include "headers/blam_data_globals.h"
#include "headers/animation_update_kind.h"


extern uint8_t game_engine_running(void);
extern int16_t game_connection(void);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);
extern uint8_t unit_animation_state_interruptable(unit_animation *animation, int16_t new_state);
extern uint8_t unit_animation_set_state(int unit_index, int16_t new_state);
extern const char *unit_get_current_weapon_label(int unit_index);
extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);
extern void unit_throw_grenade_release(int unit_index, uint8_t premature);
extern void object_start_interpolation(int object_index, int16_t frame_count);
extern void object_destroy(int object_index);
extern int unit_test_spawning(int unit_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t weapon_is_flag(int weapon_index);
extern void unit_align_facing(int unit_index, const real_vector2d *alignment_vector);


/* NOTE: `angle` (float) is arg7 — it occupies f1 and reserves/skips its GPR shadow slot (r9), so
   the prior 11-param sig's `real_vector2d *alignment_vector_unused` at r9 was that phantom shadow,
   not a real argument (disasm: r9 never read; f1 skips a GPR). body_part is arg8 (r10, extsh). */
void unit_ping_animation(int unit_index, uint8_t died, uint8_t feign_death,
                         uint8_t died_flying, uint8_t ignore_hard_pings,
                         uint8_t force_hard_pings, float angle, int16_t body_part,
                         const real_vector2d *alignment_vector, uint8_t gate)
{
    uint8_t feign_death_local = feign_death;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_animation *anim = &unit->unit.animation;  /* renamed: avoided shadowing the animation struct type */
    unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);

    char should_animate;
    uint8_t hard_ping = 0;
    float threshold;
    float accumulator;

    if ( died )
    {
        threshold = definition->unit.hard_death_threshold;
        should_animate = 1;
        feign_death_local = 0;
        if ( threshold <= 0.0f )
        {
            hard_ping = 0;
        }
        else
        {
            accumulator = unit->object.current_body_damage;
            char over = 1;
            if ( !(accumulator > threshold) )
                over = 0;
            hard_ping = over;
        }
    }
    else if ( !feign_death )
    {
        accumulator = unit->object.current_body_damage;
        float soft_threshold = definition->unit.soft_ping_threshold;
        char over_soft = 0;
        if ( accumulator > soft_threshold || unit->object.current_shield_damage > soft_threshold )
            over_soft = 1;
        threshold = definition->unit.hard_ping_threshold;
        should_animate = over_soft;
        char over = 1;
        if ( !(accumulator > threshold) )
            over = 0;
        hard_ping = over;
    }
    else
    {
        died = 1;
        should_animate = 1;
        hard_ping = 0;
    }

    int is_dead = died;
    if ( !died && (ignore_hard_pings || (unit->unit.flags & (1u << _unit_ignore_hard_pings_bit)) != 0) )
        hard_ping = 0;
    if ( force_hard_pings )
    {
        hard_ping = 1;
        should_animate = 1;
    }

    int16_t region = body_part == -1 ? 0 : (uint16_t)body_part;

    int16_t direction_bucket;  /* 0 back, 1 right, 2 left, 3 front */
    if ( __fabs(angle) >= 0.78539819f )
    {
        if ( __fabs(angle) <= 2.159845009446144 )
        {
            direction_bucket = 1;
            if ( angle <= 0.0f )
                direction_bucket = 2;
        }
        else
        {
            direction_bucket = 0;
        }
    }
    else
    {
        direction_bucket = 3;
    }
    if ( game_engine_running() && region == 2 && hard_ping && is_dead )
        direction_bucket = 1;

    if ( !gate )
        should_animate = 1;

    if ( !(should_animate || is_dead) )
        return;

    uint8_t hard_ping_or_dead_for_align = hard_ping;
    int animation_graph_index = definition->object.animation_graph.index;
    animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);
    int16_t *damage_animation_indices = (int16_t *)graph->unit_damage_animations.address;

    if ( !(hard_ping || is_dead) )
    {
        /* soft ping overlay, only if not already overlaying / overlay expired */
        if ( (uint16_t)anim->soft_ping_animation.index == 0xFFFF
          || anim->soft_ping_animation.frame_index > definition->unit.runtime_soft_ping_minimum_interrupt_ticks )
        {
            int index = (int16_t)(11 * direction_bucket + region);
            int16_t animation_index = (index < 0 || index >= graph->unit_damage_animations.count)
                ? -1 : damage_animation_indices[index];
            int16_t permutation = animation_choose_random_permutation_internal(
                animation_update_kind_affects_game_state, animation_graph_index, animation_index);
            if ( permutation != -1 )
            {
                anim->soft_ping_animation.index = permutation;
                anim->soft_ping_animation.frame_index = 0;
            }
        }
        return;
    }

    /* hard ping / death animation */
    char forced_special = 0;
    int16_t animation_state_id = is_dead == 0 ? _unit_state_hard_ping : _unit_state_dying;
    int16_t ping_type = is_dead ? (hard_ping ? 3 : 2) : 1;

    char may_animate;
    if ( is_dead || unit_animation_state_interruptable(anim, animation_state_id) )
        may_animate = 1;
    else
        may_animate = 0;
    if ( (uint8_t)anim->state == _unit_state_hard_ping
      && unit->object.animation.state.frame_index > definition->unit.runtime_hard_ping_minimum_interrupt_ticks )
        may_animate = 1;
    if ( !is_dead )
        may_animate = unit->object.parent_object_index != -1
            ? 0 : ((unit->object.damage_flags & (1u << _object_dead_bit)) != 0 ? 0 : may_animate);

    if ( !may_animate )
        return;

    if ( is_dead )
    {
        const char *seat_label = base_seat_labels[_base_seat_stand];
        const char *weapon_label = unit_get_current_weapon_label(unit_index);
        unit_set_or_test_seat_and_weapon_label(unit_index, seat_label, weapon_label, 1u);
    }

    if ( animation_state_id == _unit_state_dying && !unit->object.type )  /* death of a biped */
    {
        biped_datum *biped = (biped_datum *)unit;
        if ( (biped->biped.flags & (1u << _biped_airborne_bit)) != 0
          && (TAG_GET(biped_definition, biped->definition_index)->biped.flags & (1u << _biped_has_no_dying_airborne_bit)) == 0 )
        {
            animation_state_id = _unit_state_dying_airborne;  /* special death */
            forced_special = 1;
        }
    }

    if ( !forced_special || !unit_animation_set_state(unit_index, animation_state_id) )
    {
        int ping_type_for_table = ping_type;
        int index = (int16_t)(11 * (4 * ping_type + direction_bucket) + region);
        int16_t animation_index = (index < 0 || index >= graph->unit_damage_animations.count)
            ? -1 : damage_animation_indices[index];
        int16_t permutation = animation_choose_random_permutation_internal(
            animation_update_kind_affects_game_state, definition->object.animation_graph.index, animation_index);

        if ( permutation == -1 )
        {
            if ( is_dead )
            {
                anim->flags = anim->flags
                    & ~((1u << _unit_animation_ignore_translation_bit) | (1u << _unit_animation_fallen_on_front_bit))
                    | (1u << _unit_animation_ignore_translation_bit);
                if ( (definition->unit.flags & (1u << _unit_definition_destroyed_after_dying_bit)) != 0 )
                {
                    object_destroy(unit_index);
                    unit_test_spawning(unit_index);
                }
            }
        }
        else
        {
            if ( (uint8_t)anim->state == _unit_state_throw_grenade )
                unit_throw_grenade_release(unit_index, 1u);
            object_start_interpolation(unit_index, 3);
            anim->state = animation_state_id;
            unit->object.animation.animation_graph_index = definition->object.animation_graph.index;
            unit->object.animation.state.index = permutation;
            unit->object.animation.state.frame_index = 0;
            anim->flags |= (1u << _unit_animation_postpone_weapon_ik_until_interpolation_ends_bit);

            if ( is_dead )
            {
                if ( died_flying || feign_death_local )
                {
                    unit->unit.weapon_drop_delay_ticks = 0;
                }
                else
                {
                    char seed_twitch = 1;
                    if ( game_connection() && game_connection() != _game_connection_film_playback )
                    {
                        int weapon_object = -1;
                        int weapon_slot = unit->unit.current_weapon_index;
                        if ( weapon_slot != -1 )
                            weapon_object = unit->unit.weapon_object_indices[weapon_slot];
                        if ( object_try_and_get_and_verify_type(weapon_object, object_mask_weapon)
                          && weapon_is_flag(weapon_object) == 1 )
                        {
                            seed_twitch = 0;
                            unit->unit.weapon_drop_delay_ticks = 0;
                        }
                    }
                    if ( seed_twitch == 1 )
                    {
                        int16_t base = ((animation *)graph->animations.address)[ping_type_for_table].frame_count;
                        int low = base >> 2;
                        int high = (base >> 1) + low;
                        char twitch = seed_random_range(get_global_random_seed_address(), low, high);
                        unit->unit.weapon_drop_delay_ticks = twitch;
                        if ( twitch <= 1 )
                            twitch = 1;
                        unit->unit.weapon_drop_delay_ticks = twitch;
                    }
                }
            }

            if ( direction_bucket )
            {
                int index2 = 44 * ping_type_for_table + region;
                int16_t mapped = (index2 < 0 || index2 >= graph->unit_damage_animations.count)
                    ? -1 : damage_animation_indices[index2];
                direction_bucket = mapped == ((animation *)graph->animations.address)[ping_type_for_table].runtime_parent_animation_index
                    ? 0 : direction_bucket;
            }
            if ( is_dead )
            {
                if ( direction_bucket == 3 )
                    anim->flags |= (1u << _unit_animation_fallen_on_front_bit);
                else
                    anim->flags &= ~(1u << _unit_animation_fallen_on_front_bit);
            }
        }
    }

    /* facing realignment toward the hit, when an alignment vector was supplied */
    if ( alignment_vector
      && (definition->unit.flags & (1u << _unit_definition_dont_reface_during_pings_bit)) == 0
      && !unit->object.type
      && unit->object.parent_object_index == -1
      && (hard_ping_or_dead_for_align || is_dead) )
    {
        real_vector2d facing;
        if ( (unsigned int)direction_bucket <= 3 )
        {
            if ( !direction_bucket )  /* back */
            {
                facing.n[0] = -alignment_vector->n[0];
                facing.n[1] = -alignment_vector->n[1];
                unit_align_facing(unit_index, &facing);
                return;
            }
            if ( direction_bucket == 1 )  /* right */
            {
                facing.n[0] = -alignment_vector->n[1];
                facing.n[1] = alignment_vector->n[0];
                unit_align_facing(unit_index, &facing);
                return;
            }
            if ( direction_bucket != 2 )  /* front */
            {
                facing = *alignment_vector;
                unit_align_facing(unit_index, &facing);
                return;
            }
            /* left */
            facing.n[0] = alignment_vector->n[1];
            facing.n[1] = -alignment_vector->n[0];
        }
        unit_align_facing(unit_index, &facing);
    }
}
