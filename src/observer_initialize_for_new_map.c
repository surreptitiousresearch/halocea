/* observer_initialize_for_new_map @0x8370E8C8 — reset each local player's camera observer to a neutral state:
 * identity orientation (forward/up), 50° field of view, origin position with invalid PVS location and zero
 * velocity, a cleared command, and freshly stamped header/trailer signatures. */

#include <stdint.h>
#include <string.h>
#include "headers/observer_globals.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


/* Attested void: sole caller (game_initialize_for_new_map @83687C30) ignores r3; the decompiler's
 * threaded int result (a float->int pun off obs->up) was r3 register residue, removed. */
void observer_initialize_for_new_map(void)
{
    const real_vector3d *zero = global_zero_vector3d;
    const real_point3d *origin = global_origin3d;
    const real_vector3d *up = global_up3d;
    const real_vector3d *forward = global_forward3d;

    for ( int16_t p = 0; p < 2; p = (int16_t)(p + 1) )
    {
        observer *obs = &observer_globals.local_players[p];
        obs->forward = *forward;
        obs->up = *up;
        obs->field_of_view = 0.87266463f;

        obs->result.position = *origin;
        obs->result.location.cluster_index = -1;
        obs->result.location.leaf_index = -1;
        obs->result.velocity = *zero;
        obs->result.forward = *forward;
        obs->result.up = *up;
        obs->result.field_of_view = 0.87266463f;

        memset(&obs->last_command, 0, sizeof(obs->last_command));
        obs->last_command.field_of_view = obs->field_of_view;
        obs->last_command.forward = obs->forward;
        obs->last_command.up = obs->up;

        obs->trailer_signature = 0x72616421; /* 'rad!' */
        obs->header_signature = 0x72616421;  /* 'rad!' */
        obs->updated_for_frame = 1;
        obs->first_command = 0;
    }
}
