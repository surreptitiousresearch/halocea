/* unit_update @ 0x836D4298 — per-tick update for a unit object (biped/vehicle). Drives the full unit state
 * machine each frame: blind/garbage-controlled aiming overrides, parent/seat-inherited aiming, the
 * pause/timeout/melee/grenade/weapon-ready timers, the two aiming slews (weapon aim and looking, each toward
 * their desired vector) — each chosen between a snap-to-target, a euler-aiming update, or an angular
 * accelerator — weapon owner control flags, integrated-light ramps, deferred AI damage, dialogue, dynamic
 * illumination sampling, mouth-aperture decay, flashlight/active-camo energy, and night-vision saturation.
 * Most of the work is skipped on the simulated (rollback) update path (g_running_simulated_update). Returns 1.
 *
 * This function was decompiled with "local variable allocation has failed, the output may be wrong!"; the
 * algorithm is recoverable from the disassembly and is reconstructed faithfully here. Three call arguments
 * are fed by stale registers the decompiler could not track; each is marked DEVIATION below.
 *
 * Typed against the DB-verified unit_datum/unit_definition/weapon_definition/powered_seat_definition
 * layouts (no raw byte offsets remain). Notes on fields whose semantics differ from an earlier pass's
 * speculative comments (byte offsets given for cross-reference against the disassembly):
 *   - unit.mouth_aperture (abs +744) was previously guessed as "banking decay" -- it's a lip-sync
 *     amplitude that decays to 0 at up to 0.1/tick, which behaves identically either way, but the DB
 *     name is authoritative.
 *   - unit.gunner_inactive_ticks (abs +802) was previously guessed as a generic "firing bits" counter:
 *     it resets to 0 while the firing/special control bits (0x7C00) are set and otherwise counts up,
 *     saturating at 127.
 *   - unit.active_camouflage_super_amount (abs +896) was previously guessed as "zoom/scope-blur" --
 *     confirmed by the ramp-rate override nearby, which reads
 *     weapon_definition.weapon.active_camo_regrowth_rate.
 *   - unit_definition.unit.powered_seats (abs +716/+720) was previously guessed as a generic "lights"
 *     list; each 68-byte powered_seat_definition's powerup_time/powerdown_time (+4/+8) drives the
 *     per-seat light ramp, and the runtime intensity per seat lives in unit.seat_power[2] (abs +824/+828).
 *   - The repeated `self`/`driver`/`controlled` reloads via object_header_data are the decompiler's
 *     register-spill idiom (the same object_header_data table-lookup pattern used for `unit_object` in
 *     action_obey_command_begin.c); `self` was always identical to the already-typed `unit` and has been
 *     folded away below.
 */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/real_rgb_color.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_rectangle2d.h"
#include "headers/location.h"
#include "headers/unit_globals.h"
#include "headers/game_globals_tag.h"
#include "headers/cheats.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_animation.h"
#include "headers/unit_definition.h"
#include "headers/weapon_definition.h"
#include "headers/biped_datum.h"
#include "headers/player_datum.h"
#include "headers/powered_seat_definition.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/unit_flags.h"
#include "headers/unit_control_flags.h"
#include "headers/unit_animation_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/unit_grenade_throw_state.h"
#include "headers/unit_definition_flags.h"
#include "headers/weapon_control_flags.h"
#include "headers/unit_scream_type.h"
#include "headers/base_seat.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"

/* opaque forward declaration for a pointer-only callee argument */
typedef struct effect_vector_field effect_vector_field;


#include "headers/real_rectangle2d.h"
#include "headers/real_vector2d.h"
extern int game_engine_allow_integrated_lights(int object_index);
extern uint8_t game_engine_running(void);
extern void unit_running_blind(int unit_index, real_vector3d *run_vector);
extern uint8_t unit_drop_current_weapon(int unit_index, uint8_t immediate);
extern void unit_died(int unit_index, uint8_t feigned);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);
extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);
extern uint8_t unit_animation_set_state(int unit_index, int16_t new_state);
extern void biped_stop_limp_body_physics(int biped_index);
extern int unit_scream(int unit_index, int16_t scream_type);
extern uint8_t unit_animation_busy(unit_animation *animation);
extern uint8_t unit_can_use_weapon(int unit_index, int weapon_index);
extern void unit_ready_desired_weapon(int unit_index, uint8_t immediate);
extern int16_t unit_inventory_next_grenade(int unit_index, int16_t current_index, int16_t delta);
extern int player_index_from_unit_index(int unit_index);
extern int unspatialized_impulse_sound_new(int definition_index, float scale);
extern int unit_clip_to_aiming_bounds(int unit_index, real_vector3d *vector, uint8_t use_aiming_screen);
extern void object_get_orientation(int object_index, real_vector3d *forward, real_vector3d *up);
extern void unit_euler_aiming_update(const real_matrix4x3 *orientation, real_vector3d *aiming_vector, const real_vector3d *desired_aiming_vector, real_vector3d *aiming_velocity, const real_rectangle2d *aiming_bounds, float angular_velocity_limit, float angular_acceleration_limit);
extern void angular_accelerate_to_position(real_vector3d *position, const real_vector3d *position_desired, real_vector3d *angular_velocity, float angular_velocity_magnitude_maximum, float angular_acceleration_magnitude_maximum);
extern float angle_between_vectors3d(const real_vector3d *a, const real_vector3d *b);
extern void unit_throw_grenade_move_to_hand(int unit_index);
extern void unit_throw_grenade_release(int unit_index, uint8_t premature);
extern int unit_throw_grenade_begin(int unit_index, const real_vector2d *alignment_vector);
extern void weapon_set_integrated_light_power(int weapon_index, float light_power);
extern void weapon_owner_update(int weapon_index, uint16_t control_flags, float primary_trigger);
extern void unit_seat_update(int object_index);
extern void ai_handle_damage(int unit_index, int shooter_object_index, int16_t damage_category, float fraction, real_vector3d *damage_velocity, uint8_t delayed);
extern void unit_cause_continuous_melee_damage(int unit_index);
extern void unit_dialogue_update(int unit_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void lights_illumination_at_point(const real_point3d *point, const location *location, real_rgb_color *color);
extern float real_rgb_color_brightness(const real_rgb_color *color);
extern float object_get_self_illumination(int object_index);
extern void unit_flame_to_death(int unit_index);
extern uint8_t unit_integrated_night_vision_is_active(int unit_index);
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);

uint8_t unit_update(int unit_index)
{
    char night_vision_dirty = 0;
    char used_global_time = 0;
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit_definition *unit_def = TAG_GET(unit_definition, unit->definition_index);
    uint8_t integrated_lights_allowed = game_engine_allow_integrated_lights(unit_index);

    /* update-timer round-robin: at most one unit per tick gets the "used global time" slot */
    if ( !g_running_simulated_update )
    {
        int16_t timer = unit->unit.timer + 1;
        struct unit_globals *globals = unit_globals; /* unit_globals is a bare struct tag */
        unit->unit.timer = timer;
        if ( globals->used_time || timer <= globals->next_timer )
        {
            int16_t highest = globals->highest_timer;
            if ( highest <= unit->unit.timer )
                highest = unit->unit.timer;
            globals->highest_timer = highest;
        }
        else
        {
            globals->used_time = 1;
            used_global_time = 1;
            unit->unit.timer = 0;
        }
    }

    unsigned int unit_flags = unit->unit.flags;
    if ( (unit_flags & (1u << _unit_running_blindly_bit)) != 0 )
    {
        /* blinded: aim/look toward the running-blind drift forward */
        unit_running_blind(unit_index, &unit->unit.desired_facing_vector);
        unit->unit.desired_aiming_vector = unit->unit.desired_facing_vector;
        unit->unit.desired_looking_vector = unit->unit.desired_facing_vector;
        unit->unit.throttle = *global_forward3d;
    }
    else if ( (unit_flags & (1u << _unit_actively_controlled_bit)) == 0 )
    {
        /* fresh unit: seed all aiming/looking vectors from the object orientation */
        unit->unit.desired_facing_vector = unit->object.forward;
        unit->unit.desired_aiming_vector = unit->object.forward;
        unit->unit.desired_looking_vector = unit->object.forward;
        unit->unit.throttle = *global_zero_vector3d;
    }

    if ( (unit_flags & (1u << _unit_running_blindly_bit)) != 0 || (unit_flags & (1u << _unit_actively_controlled_bit)) == 0 )
        unit->unit.control_flags = 0;

    if ( (unit_def->unit.flags & (1u << _unit_definition_simple_creature_bit)) == 0 )
    {
        /* berserk/melee flash timer: persistent_control_timer/persistent_control_flags */
        int flash_timer = unit->unit.persistent_control_timer;
        char inherited_from_driver = 0;
        if ( flash_timer > 0 )
        {
            unsigned int flash_flags = unit->unit.persistent_control_flags;
            unsigned int new_state_flags = flash_flags | unit->unit.control_flags;
            unit->unit.control_flags = new_state_flags;
            if ( (flash_flags & (1u << _unit_control_weapon_primary_trigger_bit)) != 0 )
            {
                unsigned int adjusted = new_state_flags | (1u << _unit_control_weapon_primary_trigger_bit);
                if ( flash_timer % 7 )
                    adjusted = new_state_flags & ~(1u << _unit_control_weapon_primary_trigger_bit);
                unit->unit.primary_trigger = 1.0f;
                unit->unit.control_flags = adjusted;
            }
            else
            {
                unit->unit.primary_trigger = 0.0f;
            }
            int remaining = flash_timer - 1;
            unit->unit.persistent_control_timer = remaining;
            if ( !remaining )
                unit->unit.persistent_control_flags = 0;
        }

        unsigned int control_flags = unit->unit.flags;
        if ( (control_flags & (1u << _unit_possessed_by_recording_bit)) == 0 )
        {
            /* inherit aiming/animation state from a controlling driver and/or controlled child */
            if ( unit->unit.driver_object_index != -1 && (unit->object.damage_flags & (1u << _object_dead_bit)) == 0 )
            {
                inherited_from_driver = 1;
                unit_datum *driver = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit->unit.driver_object_index)->datum);
                unit->object.owner_team_index = driver->object.owner_team_index;
                int driver_state = driver->unit.animation.state;
                if ( driver->unit.player_index != -1 || (driver_state != _unit_state_exiting_seat && driver_state != _unit_state_entering_seat) )
                {
                    unit->unit.control_flags |= driver->unit.control_flags & UNIT_CONTROL_DRIVER_MASK;
                    unit->unit.desired_facing_vector = driver->unit.desired_facing_vector;
                    unit->unit.throttle = driver->unit.throttle;
                }
            }

            if ( unit->unit.gunner_object_index != -1 && (unit->object.damage_flags & (1u << _object_dead_bit)) == 0 )
            {
                unit_datum *controlled = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit->unit.gunner_object_index)->datum);
                if ( !inherited_from_driver )
                    unit->object.owner_team_index = controlled->object.owner_team_index;
                int controlled_state = controlled->unit.animation.state;
                if ( controlled->unit.player_index != -1 || (controlled_state != _unit_state_exiting_seat && controlled_state != _unit_state_entering_seat) )
                {
                    unsigned int prior_state_flags = unit->unit.control_flags;
                    real_vector3d look = controlled->unit.desired_aiming_vector;
                    unit->unit.desired_aiming_vector = look;
                    unit->unit.desired_looking_vector = look;
                    unit->unit.control_flags = (controlled->unit.control_flags & UNIT_CONTROL_GUNNER_MASK) | prior_state_flags;
                    unit->unit.primary_trigger = controlled->unit.primary_trigger;
                }
            }

            /* gunner_inactive_ticks: resets to 0 while the firing/special bits (0x7C00) are set,
             * otherwise counts up, saturating at 127 */
            if ( (unit->unit.control_flags & UNIT_CONTROL_GUNNER_MASK) != 0 )
                unit->unit.gunner_inactive_ticks = 0;
            else if ( unit->unit.gunner_inactive_ticks != 127 )
                ++unit->unit.gunner_inactive_ticks;
        }

        if ( !g_running_simulated_update )
        {
            /* active-camo / cloak energy ramp at +892 */
            if ( (control_flags & (1u << _unit_active_camouflaged_bit)) != 0 )
            {
                float rate = 0.0083333338f;
                if ( game_engine_running()
                    && unit->unit.cause_for_camo_regrowth
                    && unit->unit.cause_for_camo_regrowth == 1 )
                {
                    int16_t weapon_slot = unit->unit.current_weapon_index;
                    if ( weapon_slot != -1 )
                    {
                        int weapon = unit->unit.weapon_object_indices[weapon_slot];
                        if ( weapon != -1 )
                        {
                            object_datum *weapon_obj = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon)->datum;
                            weapon_definition *weapon_def = TAG_GET(weapon_definition, weapon_obj->definition_index);
                            /* DEVIATION: DB decompile reads *(tag_instance+20 (=base_address)) + 1232 — the
                             * earlier re-source double-applied the +20 to the TAG_GET result. +1232 in
                             * weapon_definition = weapon.active_camo_regrowth_rate (res.py). */
                            float tag_rate = weapon_def->weapon.active_camo_regrowth_rate;
                            if ( tag_rate != 0.0f )
                                rate = tag_rate;
                        }
                    }
                }
                float ramped = unit->unit.active_camouflage + rate;
                unit->unit.active_camouflage = ramped;
                if ( ramped > 1.0f )
                {
                    unit->unit.active_camouflage = 1.0f;
                    unit->unit.cause_for_camo_regrowth = 0;
                }
            }
            else
            {
                float ramped = unit->unit.active_camouflage - 0.0083333338f;
                unit->unit.active_camouflage = ramped;
                if ( ramped < 0.0f )
                    unit->unit.active_camouflage = 0.0f;
            }
        }

        if ( !g_running_simulated_update )
        {
            /* active_camouflage_super_amount ramp, driven by flag 0x20 */
            if ( (unit->unit.flags & (1u << _unit_super_camouflaged_bit)) != 0 )
            {
                float ramped = unit->unit.active_camouflage_super_amount + 0.011111111f;
                unit->unit.active_camouflage_super_amount = ramped;
                if ( ramped > 1.0f )
                    unit->unit.active_camouflage_super_amount = 1.0f;
            }
            else
            {
                float ramped = unit->unit.active_camouflage_super_amount - 0.011111111f;
                unit->unit.active_camouflage_super_amount = ramped;
                if ( ramped < 0.0f )
                    unit->unit.active_camouflage_super_amount = 0.0f;
            }
        }

        if ( !g_running_simulated_update )
        {
            int16_t timer = unit->unit.body_stun_ticks;
            if ( timer > 0 )
            {
                int16_t remaining = timer - 1;
                unit->unit.body_stun_ticks = remaining;
                if ( !remaining )
                    unit->unit.body_stun = 0.0f;
            }
        }

        if ( !g_running_simulated_update )
        {
            char drop_timer = unit->unit.weapon_drop_delay_ticks;
            if ( drop_timer > 0 )
            {
                char remaining = drop_timer - 1;
                unit->unit.weapon_drop_delay_ticks = remaining;
                if ( !remaining )
                    unit_drop_current_weapon(unit_index, 1u);
            }
        }

        if ( !g_running_simulated_update )
        {
            int16_t death_timer = unit->unit.feign_death_timer;
            if ( death_timer > 0 && (unit->object.flags & (1u << _object_at_rest_bit)) != 0 )
            {
                int16_t remaining = death_timer - 1;
                unit->unit.feign_death_timer = remaining;
                if ( !remaining )
                {
                    if ( unit->object.body_vitality <= 0.0f )
                    {
                        unit_died(unit_index, 0);
                    }
                    else
                    {
                        /* survived the death timer: re-stand as actively controlled, scream */
                        int16_t anim_state = (((unsigned int)~unit->unit.animation.flags >> _unit_animation_fallen_on_front_bit) & 1) | _unit_state_resurrect_front;
                        unit->object.damage_flags &= ~(1u << _object_dead_bit);
                        unit_set_actively_controlled(unit_index, 1u);
                        unit_set_or_test_seat_and_weapon_label(unit_index, base_seat_labels[_base_seat_stand], nullptr, 1u);
                        unit_animation_set_state(unit_index, anim_state);
                        int object_type = unit->object.type;
                        unit->unit.animation.flags &= ~(1u << _unit_animation_ignore_translation_bit);
                        if ( !object_type )
                            biped_stop_limp_body_physics(unit_index);
                        unit_scream(unit_index, _unit_scream_resurrection);
                    }
                }
            }
        }
    }

    if ( (unit_def->unit.flags & (1u << _unit_definition_has_no_aiming_bit)) == 0 )
    {
        if ( (unit->object.damage_flags & (1u << _object_dead_bit)) == 0 && !g_running_simulated_update )
        {
            /* weapon swap / grenade swap / ammo cheat / zoom-level transitions */
            if ( (unit->object.damage_flags & (1u << _object_cannot_hold_weapon_bit)) != 0 )
            {
                unit_drop_current_weapon(unit_index, 1u);
            }
            else if ( unit->unit.desired_weapon_index != unit->unit.current_weapon_index
                   && !unit_animation_busy(&unit->unit.animation) )
            {
                int16_t desired_slot = unit->unit.desired_weapon_index;
                if ( desired_slot != -1 )
                {
                    int weapon = unit->unit.weapon_object_indices[desired_slot];
                    if ( weapon != -1 && unit_can_use_weapon(unit_index, weapon) )
                        unit_ready_desired_weapon(unit_index, 1u);
                }
            }

            if ( unit->unit.desired_grenade_index != unit->unit.current_grenade_index
                && !unit_animation_busy(&unit->unit.animation) )
            {
                int16_t grenade = unit_inventory_next_grenade(unit_index, unit->unit.current_grenade_index, 0);
                if ( grenade != -1 )
                    unit->unit.current_grenade_index = (char)grenade;
            }

            if ( cheat.infinite_ammo && unit->unit.player_index != -1 )
            {
                for ( int i = 0; i < 2; i = (int16_t)(i + 1) )
                {
                    char ammo = unit->unit.grenade_counts[i];
                    if ( ammo <= 1 )
                        ammo = 1;
                    unit->unit.grenade_counts[i] = ammo;
                }
                if ( (uint8_t)unit->unit.desired_grenade_index == 255 )
                    unit->unit.desired_grenade_index = 0;
            }

            /* zoom-level change: play the zoom in/out sound */
            char zoom_level = unit->unit.desired_zoom_level;
            if ( zoom_level != unit->unit.current_zoom_level )
            {
                unit->unit.current_zoom_level = zoom_level;
                if ( zoom_level == -1 )
                    unit->unit.integrated_night_vision_power = 0.0f;
                if ( player_index_from_unit_index(unit_index) != -1
                    && (uint16_t)DATA_ARRAY_ELEMENT(player_data, player_datum, player_index_from_unit_index(unit_index))
                        ->local_player_index != 0xFFFF )   /* DEVIATION: int16_t field — uncast, the promoted -1 made this always TRUE, so the zoom-change HUD refresh ran for non-local players; binary zero-extends, lhz r9,2(r10) @0x836D4B24 + cmplwi cr6,r9,0xFFFF @0x836D4B28 */
                {
                    int16_t weapon_slot = unit->unit.current_weapon_index;
                    if ( weapon_slot != -1 )
                    {
                        int weapon = unit->unit.weapon_object_indices[weapon_slot];
                        if ( weapon != -1 )
                        {
                            char level = unit->unit.current_zoom_level;
                            object_datum *weapon_obj = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon)->datum;
                            weapon_definition *weapon_def = TAG_GET(weapon_definition, weapon_obj->definition_index);
                            int sound;
                            if ( level == -1 )
                                sound = weapon_def->weapon.zoom_out_sound.index;
                            else
                                sound = weapon_def->weapon.zoom_in_sound.index;
                            float pitch = 1.0f;
                            if ( level != -1 )
                            {
                                int16_t zoom_levels = weapon_def->weapon.zoom_level_count;
                                if ( zoom_levels > 1 )
                                    pitch = (float)level / (float)(zoom_levels - 1);
                            }
                            if ( sound != -1 )
                                unspatialized_impulse_sound_new(sound, pitch);
                        }
                    }
                }
            }
        }

        /* ---- weapon aiming slew (aiming_vector toward desired_aiming_vector), velocity at aiming_velocity ---- */
        float speed_scale;
        if ( unit->unit.aiming_speed == 1 )
            speed_scale = unit_def->unit.casual_aiming_modifier;   /* crouched aim-speed scale */
        else
            speed_scale = 1.0f;
        float aim_velocity_max = (unit_def->unit.aiming_velocity_maximum * speed_scale * SECONDS_PER_TICK);
        float aim_acceleration_max = (unit_def->unit.aiming_acceleration_maximum * speed_scale * 0.0011111111f);

        real_vector3d previous_aim = unit->unit.aiming_vector;

        if ( aim_velocity_max == 0.0f && aim_acceleration_max == 0.0f )
        {
            /* instantaneous aim: snap to desired and clip to bounds (unless free in a controlled seat) */
            char clip = 1;
            unit->unit.aiming_vector = unit->unit.desired_aiming_vector;
            if ( game_engine_running() && unit_index != -1 )
            {
                if ( unit->unit.player_index != -1
                    && (unit->object.parent_object_index == -1 || unit->unit.parent_seat_index == (int16_t)0xFFFF) )
                    clip = 0;
            }
            if ( clip )
                unit_clip_to_aiming_bounds(unit_index, &unit->unit.aiming_vector, 1u);
            unit->unit.aiming_velocity = *global_zero_vector3d;
        }
        else if ( unit->unit.animation.aiming_with_euler_screen )
        {
            real_matrix4x3 frame;
            frame.scale = 1.0f;
            object_get_orientation(unit_index, (real_vector3d *)frame.n[0], (real_vector3d *)frame.n[2]);
            frame.n[1][0] = (frame.n[0][2] * frame.n[2][1]) - (frame.n[2][2] * frame.n[0][1]);
            frame.n[1][1] = (frame.n[2][2] * frame.n[0][0]) - (frame.n[0][2] * frame.n[2][0]);
            frame.n[1][2] = (frame.n[0][1] * frame.n[2][0]) - (frame.n[2][1] * frame.n[0][0]);
            frame.n[3][0] = global_origin3d->n[0];
            frame.n[3][1] = global_origin3d->n[1];
            frame.n[3][2] = global_origin3d->n[2];
            unit_euler_aiming_update(&frame, &unit->unit.aiming_vector, &unit->unit.desired_aiming_vector,
                &unit->unit.aiming_velocity, &unit->unit.animation.aiming_screen_bounds,
                aim_velocity_max, aim_acceleration_max);
        }
        else
        {
            angular_accelerate_to_position(&unit->unit.aiming_vector, &unit->unit.desired_aiming_vector,
                &unit->unit.aiming_velocity, aim_velocity_max, aim_acceleration_max);
        }

        /* aiming_change: how far (0..255) the aim moved this tick relative to the per-tick max, for muzzle sway */
        float aim_fraction;
        if ( unit_def->unit.aiming_velocity_maximum == 0.0f )
        {
            aim_fraction = 0.0f;
        }
        else
        {
            aim_fraction = (angle_between_vectors3d(&unit->unit.aiming_vector, &previous_aim)
                / (unit_def->unit.aiming_velocity_maximum * SECONDS_PER_TICK));
            if ( aim_fraction < 0.0f )
                aim_fraction = 0.0f;
            else if ( aim_fraction > 1.0f )
                aim_fraction = 1.0f;
        }
        unit->unit.aiming_change = (uint8_t)(int64_t)(aim_fraction * 255.0f);

        /* ---- looking slew (looking_vector toward desired_looking_vector), velocity at looking_velocity ---- */
        float look_velocity_max = (unit_def->unit.looking_velocity_maximum * speed_scale * SECONDS_PER_TICK);
        float look_acceleration_max = (unit_def->unit.looking_acceleration_maximum * speed_scale * 0.0011111111f);
        if ( look_velocity_max == 0.0f && look_acceleration_max == 0.0f )
        {
            unit->unit.looking_vector = unit->unit.desired_looking_vector;
            unit_clip_to_aiming_bounds(unit_index, &unit->unit.looking_vector, 0);
            unit->unit.looking_velocity = *global_zero_vector3d;
        }
        else if ( unit->unit.animation.looking_with_euler_screen )
        {
            real_matrix4x3 frame;
            frame.scale = 1.0f;
            object_get_orientation(unit_index, (real_vector3d *)frame.n[0], (real_vector3d *)frame.n[2]);
            frame.n[1][0] = (frame.n[0][2] * frame.n[2][1]) - (frame.n[2][2] * frame.n[0][1]);
            frame.n[1][1] = (frame.n[2][2] * frame.n[0][0]) - (frame.n[0][2] * frame.n[2][0]);
            frame.n[1][2] = (frame.n[0][1] * frame.n[2][0]) - (frame.n[2][1] * frame.n[0][0]);
            frame.n[3][0] = global_origin3d->n[0];
            frame.n[3][1] = global_origin3d->n[1];
            frame.n[3][2] = global_origin3d->n[2];
            unit_euler_aiming_update(&frame, &unit->unit.looking_vector, &unit->unit.desired_looking_vector,
                &unit->unit.looking_velocity, &unit->unit.animation.looking_screen_bounds,
                look_velocity_max, look_acceleration_max);
        }
        else
        {
            angular_accelerate_to_position(&unit->unit.looking_vector, &unit->unit.desired_looking_vector,
                &unit->unit.looking_velocity, look_velocity_max, look_acceleration_max);
        }

        if ( !g_running_simulated_update )
        {
            /* grenade-throw state machine */
            uint8_t throw_state = unit->unit.grenade_throw_state;
            if ( throw_state <= _grenade_throw_state_released )
            {
                if ( throw_state == _grenade_throw_state_begin )
                {
                    if ( unit->object.animation.state.frame_index >= 2 )
                        unit_throw_grenade_move_to_hand(unit_index);
                }
                else if ( throw_state == _grenade_throw_state_in_hand )
                {
                    int anim_state = (uint8_t)unit->unit.animation.state;
                    ++unit->unit.grenade_throw_ticks;
                    if ( anim_state != _unit_state_throw_grenade )
                        unit_throw_grenade_release(unit_index, 1u);
                }
                else if ( unit->unit.grenade_throw_state )
                {
                    if ( unit->unit.animation.state != _unit_state_throw_grenade && ((unit->unit.control_flags >> _unit_control_throw_grenade_bit) & 1) == 0 )
                        unit->unit.grenade_throw_state = _grenade_throw_state_none;
                }
                else if ( ((unit->unit.control_flags >> _unit_control_throw_grenade_bit) & 1) != 0 )
                {
                    unit_throw_grenade_begin(unit_index, nullptr);
                }
            }
        }

        /* ---- weapon owner control flags ---- */
        int16_t weapon_slot = unit->unit.current_weapon_index;
        if ( weapon_slot != -1 && !g_running_simulated_update )
        {
            float primary_trigger = unit->unit.primary_trigger;
            unsigned int control_flags = 0;
            if ( weapon_slot == unit->unit.desired_weapon_index )
            {
                char active_camo = 0;
                if ( unit->unit.persistent_control_timer > 0 && (unit->unit.persistent_control_flags & (1u << _unit_control_weapon_primary_trigger_bit)) != 0 )
                    active_camo = 1;
                if ( integrated_lights_allowed && (unit->unit.control_flags & (1u << _unit_control_integrated_light_bit)) != 0 )
                    control_flags = (1u << _weapon_control_integrated_light_bit);
                unsigned int state_flags = unit->unit.control_flags;
                if ( (state_flags & (1u << _unit_control_weapon_primary_trigger_bit)) != 0 )
                    control_flags |= (1u << _weapon_control_primary_trigger_bit);
                if ( (state_flags & (1u << _unit_control_weapon_secondary_trigger_bit)) != 0 )
                    control_flags |= (1u << _weapon_control_secondary_trigger_bit);
                if ( (unit_def->unit.flags & (1u << _unit_integrated_light_controls_weapon_directly)) != 0 )
                {
                    int weapon = -1;
                    int16_t slot = unit->unit.current_weapon_index;
                    if ( slot != -1 )
                        weapon = unit->unit.weapon_object_indices[slot];
                    weapon_set_integrated_light_power(weapon, unit->unit.integrated_light_power);
                }
                if ( (unit->unit.control_flags & (1u << _unit_control_weapon_reload_bit)) != 0 )
                    control_flags |= (1u << _weapon_control_reload_bit);
                if ( unit_animation_busy(&unit->unit.animation) && !active_camo )
                    control_flags |= (1u << _weapon_control_user_busy_bit);
                /* biped-only: object.type == object_type_biped guards the _biped_datum extension read */
                if ( !unit->object.type && ((biped_datum *)unit)->biped.player_melee_ticks > 0 )
                    control_flags |= (1u << _weapon_control_user_busy_bit);
                if ( (uint8_t)unit->unit.current_zoom_level != 255 )
                    control_flags |= (1u << _weapon_control_zoomed_bit);
            }
            else
            {
                control_flags = (1u << _weapon_control_user_switching_weapons_bit);
            }
            int weapon = -1;
            int16_t slot = unit->unit.current_weapon_index;
            if ( slot != -1 )
                weapon = unit->unit.weapon_object_indices[slot];
            weapon_owner_update(weapon, control_flags, primary_trigger);
        }
    }

    if ( (unit_def->unit.flags & (1u << _unit_definition_simple_creature_bit)) == 0 )
    {
        /* seat acceleration jostle: seat_acceleration smoothed from seat_desired_acceleration */
        if ( (unit->unit.animation.flags & (1u << _unit_animation_showing_acceleration_bit)) != 0 )
        {
            unit_seat_update(unit_index);
            float prev_left = unit->unit.seat_desired_acceleration.n[1];
            float prev_up = unit->unit.seat_desired_acceleration.n[2];
            unit->unit.seat_acceleration.n[0] = (unit->unit.seat_desired_acceleration.n[0] * 0.30000001f)
                + (unit->unit.seat_acceleration.n[0] * 0.69999999f);
            unit->unit.seat_acceleration.n[1] = (prev_left * 0.30000001f) + (unit->unit.seat_acceleration.n[1] * 0.69999999f);
            unit->unit.seat_acceleration.n[2] = (prev_up * 0.30000001f) + (unit->unit.seat_acceleration.n[2] * 0.69999999f);
        }

        /* per-seat integrated-light intensity ramp, driven by powered_seats (powerup/powerdown_time)
         * and the runtime intensity in unit.seat_power[2] */
        int light_count = unit_def->unit.powered_seats.count;
        if ( light_count > 0 )
        {
            powered_seat_definition *seats = (powered_seat_definition *)unit_def->unit.powered_seats.address;
            for ( int light_index = 0; light_index < light_count; light_index = (int16_t)(light_index + 1) )
            {
                powered_seat_definition *seat = &seats[light_index];
                char light_on;
                if ( light_index == 0 )
                    light_on = (unit->unit.driver_object_index != -1 || (unit->unit.flags & (1u << _unit_actively_controlled_bit)) != 0);
                else
                {
                    int controlled = unit->unit.gunner_object_index;
                    light_on = (controlled != -1 && controlled != unit->unit.driver_object_index);
                }
                char enabled = (unit->object.damage_flags & (1u << _object_dead_bit)) != 0 ? 0 : light_on;
                float *intensity = (float *)((char *)&unit->unit.seat_power[0] + 4 * light_index);
                if ( enabled )
                {
                    if ( *intensity != 1.0f )
                    {
                        *intensity += (1.0f / (seat->powerup_time * 30.0f));
                        if ( *intensity > 1.0f )
                            *intensity = 1.0f;
                    }
                }
                else if ( *intensity != 0.0f )
                {
                    *intensity -= (1.0f / (seat->powerdown_time * 30.0f));
                    if ( *intensity < 0.0f )
                        *intensity = 0.0f;
                }
            }
        }
    }

    /* deferred AI damage notification */
    int16_t damage_timer = unit->unit.delayed_damage_timer;
    if ( damage_timer > 0 )
    {
        int16_t remaining = damage_timer - 1;
        unit->unit.delayed_damage_timer = remaining;
        if ( !remaining )
        {
            /* DEVIATION: ai_handle_damage's `float fraction` consumes the r6 GPR slot as well as f1, so
             * damage_velocity is r7 and delayed is r8 — not r6/r7. r6 is the float's dead shadow, which is
             * why it is unset at the call site; the values are not unrecoverable at all. 0x836D534C-
             * 0x836D5354 sets r8 = 1 and r7 = 0: damage_velocity = NULL (proven, not assumed) and
             * delayed = 1. A prior revision read the decompiler's one-register-left mapping, took the
             * unset r6 for damage_velocity, and passed delayed = 0 — the opposite of the flag this
             * deferred-notification path exists to raise. */
            ai_handle_damage(unit_index, unit->unit.delayed_damage_attacker_object_index,
                unit->unit.last_damage_category, unit->unit.delayed_damage_peak, nullptr, 1);
            unit->unit.delayed_damage_peak = 0.0f;
            unit->unit.last_damage_category = 0;
            unit->unit.delayed_damage_attacker_object_index = -1;
        }
    }

    if ( !g_running_simulated_update )
        unit_cause_continuous_melee_damage(unit_index);
    if ( !g_running_simulated_update )
        unit_dialogue_update(unit_index);

    /* dynamic illumination sample */
    if ( !g_running_simulated_update && (used_global_time || unit->unit.player_index != -1) )
    {
        unit_datum *parent_light = object_try_and_get_and_verify_type(unit->object.parent_object_index, object_mask_unit);
        if ( parent_light )
        {
            unit->unit.ambient_illumination = parent_light->unit.ambient_illumination;
            unit->unit.self_illumination = parent_light->unit.self_illumination;
        }
        else
        {
            real_rgb_color illumination;
            lights_illumination_at_point(&unit->object.position, &unit->object.location, &illumination); /* location is a value member; pass its address */
            unit->unit.ambient_illumination = real_rgb_color_brightness(&illumination);
            unit->unit.self_illumination = object_get_self_illumination(unit_index);
        }
    }

    /* flame-to-death countdown */
    char flame_timer = unit->unit.flaming_death_delay;
    if ( flame_timer && !g_running_simulated_update )
    {
        char remaining = flame_timer - 1;
        unit->unit.flaming_death_delay = remaining;
        if ( !remaining )
            unit_flame_to_death(unit_index);
    }

    /* mouth_aperture decay toward 0, clamped per tick to +/-0.1 */
    if ( !g_running_simulated_update )
    {
        float delta = -unit->unit.mouth_aperture;
        if ( delta < -0.1f )
            delta = -0.1f;
        else if ( delta > 0.1f )
            delta = 0.1f;
        unit->unit.mouth_aperture = delta + unit->unit.mouth_aperture;
    }

    if ( g_running_simulated_update )
        return 1;

    /* flashlight on/off edge-trigger flags (0x10000000 / 0x20000000) versus the persisted state (0x80000) */
    unsigned int flags1 = unit->unit.flags;
    if ( (flags1 & (1u << _unit_desired_integrated_light_on_bit)) != 0 )
    {
        if ( (flags1 & (1u << _unit_integrated_light_on_bit)) == 0 )
            night_vision_dirty = 1;
        unit->unit.flags = flags1 & ~(1u << _unit_desired_integrated_light_on_bit);
    }
    unsigned int flags2 = unit->unit.flags;
    if ( (flags2 & (1u << _unit_desired_integrated_light_off_bit)) != 0 )
    {
        if ( (flags2 & (1u << _unit_integrated_light_on_bit)) != 0 )
            night_vision_dirty = 1;
        unit->unit.flags = flags2 & ~(1u << _unit_desired_integrated_light_off_bit);
    }

    unsigned int firing_bit = unit->unit.control_flags & (1u << _unit_control_integrated_light_bit);
    if ( firing_bit || unit->unit.integrated_light_battery <= 0.0f || night_vision_dirty )
    {
        if ( integrated_lights_allowed )
        {
            if ( !unit_integrated_night_vision_is_active(unit_index) )
                goto flashlight_effect;
            if ( firing_bit )
            {
                /* DEVIATION: effect_new_from_object color/impulse_field args are stale registers (r7/r8 not
                 * set at the call site); unrecoverable, so NULL/NULL are passed. */
                int *fp_interface = global_game_globals->first_person_interface.address;
                int effect;
                if ( (unit->unit.flags & (1u << _unit_integrated_night_vision_on_bit)) != 0 )
                    effect = fp_interface[25];   /* night-vision deactivate effect */
                else
                    effect = fp_interface[21];   /* night-vision activate effect */
                if ( effect != -1 )
                    effect_new_from_object(effect, unit_index, unit_index, -1, 0.0f, 0.0f, nullptr, nullptr);
                unit->unit.flags ^= (1u << _unit_integrated_night_vision_on_bit);
            }
            if ( (unit->unit.control_flags & (1u << _unit_control_integrated_light_bit)) == 0 )
            {
flashlight_effect:
                if ( ((unit->unit.flags & (1u << _unit_integrated_light_on_bit)) != 0 || unit->unit.integrated_light_battery > 0.2f)
                    && unit->object.parent_object_index == -1 )
                {
                    /* DEVIATION: stale color/impulse_field args (see above) passed as NULL/NULL. */
                    effect_new_from_object(unit_def->unit.integrated_light_toggle_effect.index, unit_index,
                        unit_index, -1, 0.0f, 0.0f, nullptr, nullptr);
                    unit->unit.flags ^= (1u << _unit_integrated_light_on_bit);
                }
            }
        }
        else
        {
            unsigned int flags = unit->unit.flags;
            if ( (flags & (1u << _unit_integrated_night_vision_on_bit)) != 0 )
                unit->unit.flags = flags & ~(1u << _unit_integrated_night_vision_on_bit);
            flags = unit->unit.flags;
            if ( (flags & (1u << _unit_integrated_light_on_bit)) != 0 )
                /* orig `& 0xFFF7FFEF | 0x10`: clears integrated_light_on, sets active_camouflaged */
                unit->unit.flags = (flags & ~((1u << _unit_active_camouflaged_bit) | (1u << _unit_integrated_light_on_bit)))
                                 | (1u << _unit_active_camouflaged_bit);
        }
    }

    /* flashlight energy (integrated_light_battery) and beam fade (integrated_light_power) */
    unsigned int flashlight_flags = unit->unit.flags;
    if ( (flashlight_flags & (1u << _unit_integrated_light_on_bit)) != 0 )
    {
        if ( (unit_def->unit.flags & (1u << _unit_integrated_light_fucking_lasts_forever)) == 0 )
            unit->unit.integrated_light_battery = unit->unit.integrated_light_battery - 0.00027777778f;
        if ( unit->object.parent_object_index != -1 || (unit->object.damage_flags & (1u << _object_dead_bit)) != 0 )
            unit->unit.flags = flashlight_flags & ~(1u << _unit_integrated_light_on_bit);
        if ( unit->unit.integrated_light_power != 1.0f )
        {
            unit->unit.integrated_light_power = unit->unit.integrated_light_power + 0.16666667f;
            if ( unit->unit.integrated_light_power > 1.0f )
                unit->unit.integrated_light_power = 1.0f;
        }
    }
    else
    {
        if ( unit->unit.integrated_light_battery < 1.0f )
            unit->unit.integrated_light_battery = unit->unit.integrated_light_battery + 0.0011111111f;
        if ( unit->unit.integrated_light_power != 0.0f )
        {
            unit->unit.integrated_light_power = unit->unit.integrated_light_power - 0.041666668f;
            if ( unit->unit.integrated_light_power < 0.0f )
                unit->unit.integrated_light_power = 0.0f;
        }
    }

    /* night-vision saturation fade (integrated_night_vision_power) */
    if ( !unit_integrated_night_vision_is_active(unit_index) )
        return 1;
    float saturation = unit->unit.integrated_night_vision_power;
    if ( (unit->unit.flags & (1u << _unit_integrated_night_vision_on_bit)) != 0 )
    {
        if ( saturation != 1.0f )
        {
            unit->unit.integrated_night_vision_power = unit->unit.integrated_night_vision_power + 0.083333336f;
            if ( unit->unit.integrated_night_vision_power > 1.0f )
                unit->unit.integrated_night_vision_power = 1.0f;
        }
        return 1;
    }
    if ( saturation == 0.0f )
        return 1;
    /* DEVIATION: the fade-out decrement was a stale register (fp12) the decompiler lost; the symmetric
     * fade-in constant 0.083333336 (__real_3daaaaab) is substituted. */
    unit->unit.integrated_night_vision_power = unit->unit.integrated_night_vision_power - 0.083333336f;
    if ( unit->unit.integrated_night_vision_power < 0.0f )
        unit->unit.integrated_night_vision_power = 0.0f;
    return 1;
}
