/* observer_update @ 0x837105F8 — advance each local player's observer (the resolved, smoothed camera)
 * for the frame. For every active local player it consumes the pending command (observer_update_command),
 * integrates the camera motion over elapsed time (observer_pass_time) and runs the post-move checks
 * (observer_postcheck, e.g. collision/clipping).
 *
 * When local-player movement prediction is requested it is suppressed unless the player is a biped that
 * is unparented (object.parent_object_index == -1) and the camera is genuinely first person. If still
 * enabled, the biped's predicted sub-tick displacement is added to the resolved camera position so the
 * view tracks the player smoothly between simulation ticks. (The dword at biped word[71] is
 * object.parent_object_index — prediction is suppressed while the biped is parented to another object.)
 *
 * Deviation: the decompiler split the position add into scalar temporaries (v11/v12); restored to a
 * direct component-wise add. Player datum stride is 512 bytes; +52 holds the player's unit object index. */

#include <stdint.h>
#include "headers/object_type.h"
#include "headers/observer_globals.h"
#include "headers/player_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern float game_time_get_since_tick(void);
extern void observer_update_command(int16_t local_player_index);
extern void observer_pass_time(int16_t local_player_index);
extern void observer_postcheck(int16_t local_player_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t director_is_first_person(int16_t local_player_index);
extern uint8_t biped_predicted_movement_localplayer(real_point3d *position, real_vector3d *forward, real_vector3d *up, float time_since_tick_sec);

void observer_update(float dt, uint8_t predict_local_player_movement)
{
    int local_player;

    observer_globals.dtime = dt;   /* disasm: single param r4=predict flag; DB "interpolate" is spurious; dt is float (stfs) */

    for ( local_player = 0; local_player < 2; local_player++ )
    {
        int player_index = local_player_get_player_index((int16_t)local_player);
        float since_tick;
        observer *obs;

        if ( player_index == -1 )
            continue;

        since_tick = game_time_get_since_tick();
        obs = &observer_globals.local_players[local_player];
        obs->updated_for_frame = 1;

        observer_update_command(player_index);
        if ( observer_globals.dtime != 0.0f )
            observer_pass_time(player_index);
        observer_postcheck(player_index);

        if ( predict_local_player_movement )
        {
            int unit_index =
                DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
            if ( unit_index != -1 )
            {
                object_datum *biped = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
                if ( biped && biped->object.parent_object_index != -1 )  /* parented (e.g. riding a vehicle): no prediction */
                    predict_local_player_movement = 0;
            }
        }
        if ( predict_local_player_movement && !director_is_first_person(player_index) )
            predict_local_player_movement = 0;

        if ( predict_local_player_movement )
        {
            real_point3d  position_delta;
            real_vector3d forward_up[2];
            real_vector3d velocity;
            if ( biped_predicted_movement_localplayer(&position_delta, forward_up, &velocity, since_tick) )
            {
                obs->result.position.n[0] += position_delta.n[0];
                obs->result.position.n[1] += position_delta.n[1];
                obs->result.position.n[2] += position_delta.n[2];
            }
        }
    }
}
