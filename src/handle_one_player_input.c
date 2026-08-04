/* handle_one_player_input @ 0x836DFFE8 — translate one local player's raw input blob into the
 * authoritative player_control state for the tick, then queue the resulting player_action for the
 * network/film client. Skipped entirely when a co-op guest owns input (hcex_coop_local_player_index > 0).
 *
 * Steps: fetch the input blob into a scratch player_action; zero it if the director is inhibiting input
 * (cutscene); apply HCEX cheats (always-shoot forces the trigger and the fire bit); handle debug unit
 * possession (connection 0 only); reconcile the desired weapon/grenade against the unit's actual
 * inventory and any forced-ready weapon; rotate zoom level; fold in look angles unless facing is
 * inhibited; run the auto-level heuristic; commit to player_control; and finally repack into a
 * player_action for update_client_queue.
 *
 * Deviation: the decompiler rendered the action<->player_control transfers as cross-struct field
 * assignments because the two layouts share compatible prefixes — these are raw 8-byte / dword
 * reinterpretations, preserved here as the database typed them. The 0xFAFAFAFA debug-fill unrolled loop
 * is restored to a memset; the object-header indexing (3*idx+2 ints, +8 = unit data pointer) is the
 * standard datum-address pattern. */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/player_action.h"
#include "headers/game_globals_tag.h"
#include "headers/real_vector2d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/game_connection.h"
#include "headers/unit_control_flags.h"
#include "headers/blam_data_globals.h"

typedef struct input_blob input_blob;

extern void *memset(void *dst, int c, unsigned int n);


extern void get_local_player_input_blob(int16_t local_player_index, float seconds_elapsed, input_blob *input);
extern int local_player_get_player_index(int16_t local_player_index);
extern uint8_t director_inhibited_input(int16_t local_player_index);
extern uint8_t director_inhibited_facing(int16_t local_player_index);
extern int16_t game_connection(void);
extern int units_debug_get_next_unit(int unit_index);
extern int units_debug_get_closest_unit(int unit_index);
extern void players_set_local_player_unit(int16_t local_player_index, int new_unit_index);
extern void unit_debug_ninja_rope(int unit_index);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern int16_t unit_inventory_next_weapon(int unit_index, int16_t current_index, int16_t delta);
extern int16_t unit_inventory_get_must_be_readied_weapon(int unit_index);
extern int16_t unit_get_grenade_count(int unit_index, int16_t grenade_type);
extern int16_t unit_inventory_next_grenade(int unit_index, int16_t current_index, int16_t delta);
extern uint8_t player_control_camera_control_is_active(void);
extern uint8_t cinematic_in_progress(void);
extern int16_t weapon_rotate_zoom_level(int weapon_index, int16_t zoom_level);
extern void player_control_modify_desired_angles(int16_t local_player_index, float delta_yaw, float delta_pitch);
extern uint8_t player_ui_autolevel_enabled(int16_t controller_index);
extern double __fabs(double x);
extern void update_client_queue(const player_action *action, int ticks_to_apply_action_to);

/* DEVIATION: the reconstruction gave this four params (a phantom input_blob* at slot 3 and a double
 * seconds_elapsed). Disasm reads only r3 (int16 local_player_index), f1 (float seconds_elapsed, so r4 is
 * skipped) and r5 (int ticks_to_apply_action_to) — three params. The phantom pointer is dropped. */
void handle_one_player_input(int16_t local_player_index, float seconds_elapsed,
                             int ticks_to_apply_action_to)
{
    player_control *control = &player_control_globals->players[local_player_index];
    __int16 *player_control_tag = (__int16 *)global_game_globals->player_control.address;
    player_action action;
    unsigned int control_flags_out;
    float primary_trigger_out;
    char buttons;

    if ( hcex_coop_local_player_index > 0 )
        return;

    memset(&action, 0xFA, sizeof(player_action));   /* debug fill (0xFAFAFAFA) */
    /* arg order corrected to the disasm-attested get_local_player_input_blob(lp, seconds, input):
       the reconstruction scrambled it (seconds_elapsed appeared as a 4th arg, ticks_* at arg2).
       r5 at the call (0x836E0054) is the memset'd `action` buffer, reused as the input_blob. */
    get_local_player_input_blob(local_player_index, seconds_elapsed, (input_blob *)&action);
    local_player_get_player_index(local_player_index);

    if ( director_inhibited_input(local_player_index) )
        memset(&action, 0, sizeof(player_action));  /* cutscene: discard input */

    if ( hcex_always_shoot )
    {
        control_flags_out = *(unsigned int *)&action.desired_weapon_index | (1u << _unit_control_weapon_primary_trigger_bit);
        primary_trigger_out = 1.0f;
    }
    else
    {
        control_flags_out = *(unsigned int *)&action.desired_weapon_index;
        primary_trigger_out = action.desired_facing.n[1];
    }

    buttons = (char)action.pad;
    if ( game_connection() == _game_connection_local )
    {
        /* debug unit possession */
        if ( (action.pad & 8) != 0 || (action.pad & 0x10) != 0 )
        {
            int target = (action.pad & 0x10) != 0
                       ? units_debug_get_next_unit(control->unit_index)
                       : units_debug_get_closest_unit(control->unit_index);
            if ( target != -1 )
                players_set_local_player_unit(local_player_index, target);
        }
        if ( (buttons & 0x20) != 0 && control->unit_index != -1 )
            unit_debug_ninja_rope(control->unit_index);
    }

    if ( control->unit_index != -1 )
    {
        /* object-header datum: 12-byte records of ints; element +8 (int index 2) is the unit data ptr */
        unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, control->unit_index)->datum);
        int current_weapon = unit_inventory_get_weapon(control->unit_index, unit->unit.current_weapon_index);
        int desired_weapon = current_weapon;
        int desired_weapon_index = (unsigned __int16)control->desired_weapon_index;

        if ( desired_weapon_index == 0xFFFF
          || unit_inventory_get_weapon(control->unit_index, desired_weapon_index) == -1 )
            control->desired_weapon_index = unit->unit.desired_weapon_index;

        if ( (buttons & 1) != 0
          || unit_inventory_get_weapon(control->unit_index, control->desired_weapon_index) == -1
          || (unsigned __int16)control->desired_weapon_index == 0xFFFF )
        {
            control->desired_weapon_index =
                unit_inventory_next_weapon(control->unit_index, control->desired_weapon_index, buttons & 1);
            control->desired_zoom_level = -1;
        }

        {
            __int16 must_be_readied = unit_inventory_get_must_be_readied_weapon(control->unit_index);
            if ( must_be_readied != -1 && control->desired_weapon_index != must_be_readied )
            {
                control->desired_weapon_index = must_be_readied;
                control->desired_zoom_level = -1;
            }
        }

        if ( (unsigned __int16)control->desired_grenade_index == 0xFFFF
          || !unit_get_grenade_count(control->unit_index, control->desired_grenade_index) )
            control->desired_grenade_index = unit->unit.desired_grenade_index;

        if ( (buttons & 2) != 0
          || !unit_get_grenade_count(control->unit_index, control->desired_grenade_index)
          || (unsigned __int16)control->desired_grenade_index == 0xFFFF )
            control->desired_grenade_index =
                unit_inventory_next_grenade(control->unit_index, control->desired_grenade_index, 1);

        if ( (buttons & 4) != 0 && player_control_camera_control_is_active()
          && current_weapon != -1 && !cinematic_in_progress() )
            control->desired_zoom_level = weapon_rotate_zoom_level(current_weapon, control->desired_zoom_level);

        if ( !director_inhibited_facing(local_player_index) )
            player_control_modify_desired_angles(local_player_index,
                                                 action.throttle.n[0], action.throttle.n[1]);

        /* auto-level: when on foot, holding a sustained sideways throttle with no look input slowly
         * re-levels the pitch once a threshold tick count is reached */
        if ( unit->object.parent_object_index == -1 )   /* not in a vehicle/seat */
        {
            if ( player_ui_autolevel_enabled(local_player_index)
              && __fabs(control->throttle.n[0]) > 0.5
              && action.throttle.n[1] < 0.000099999997
              && control->magnetism_level < 0.000099999997 )
            {
                int ticks = control->autolevel_ticks + 1;
                if ( ticks < 0 )       ticks = 0;
                else if ( ticks > 127 ) ticks = 127;
                control->autolevel_ticks = (char)ticks;
                /* branchless signed >= : auto-level once accumulated ticks reach the tag threshold */
                control->use_autolevel =
                    (((((char)ticks ^ player_control_tag[55]) >= 0)
                      + (player_control_tag[55] >= (unsigned int)(char)ticks)) & 1);
                goto commit;
            }
            control->autolevel_ticks = 0;
        }
        control->use_autolevel = 0;
    }

commit:
    control->primary_trigger = primary_trigger_out;
    control->control_flags = control_flags_out;
    control->throttle = *(real_vector2d *)&action.control_flags;

    if ( local_player_get_player_index(local_player_index) != -1 )
    {
        /* repack player_control into a player_action for the client queue */
        real_euler_angles2d facing = control->desired_angles;
        *(unsigned int *)&facing.n[0] = control->control_flags;
        action.primary_trigger = control->primary_trigger;
        action.desired_facing = facing;
        action.control_flags = *(unsigned int *)&facing.n[0];
        action.desired_weapon_index = control->desired_weapon_index;
        action.desired_grenade_index = control->desired_grenade_index;
        action.desired_zoom_level = control->desired_zoom_level;
        action.throttle = control->throttle;
        update_client_queue(&action, ticks_to_apply_action_to);
    }
}
