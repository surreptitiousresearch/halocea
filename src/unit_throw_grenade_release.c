/* unit_throw_grenade_release @0x836CB8D8 — release the grenade a unit is mid-throw (state +653 == 2).
 * Detaches the held grenade projectile (+660), computes its launch velocity and release position, applies it,
 * advances the throw state to 3, and (on a server) replicates the new projectile to all machines.
 *
 * Two velocity sources: a player-controlled unit (+500 actor == -1) derives a launch frame from its throw
 * velocity (+572..+580), the global up vector and the player-information throw offsets; an AI unit lets
 * actor_aim_grenade compute the aim from the grenade origin. When released prematurely the velocity is blended
 * toward a weak random toss by the fraction of the throw animation completed.
 *
 * Deviation: the decompiler garbled the premature-throw ratio (OVERLAPPED __int64 locals, "allocation
 * failed"); disassembly at 0x836CBB20 shows it is (float)(__int16)*(object_data+654) /
 * (float)(__int16)*(object_data+656). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/game_globals_player_information.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/game_globals_tag.h"
#include "headers/networked_datum_role.h"
#include "headers/game_connection.h"
#include "headers/unit_grenade_throw_state.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"
/* DEVIATION: the decompiler modeled player_information.address@0; the DB-verified matg layout
 * (game_globals_tag.h) places player_information (tag_block) @0x170 with address@0x174 — disasm reads
 * `lwz r26, 0x174(r11)`. Reusing the canonical header corrects the offset. */


#include "headers/network_game_server.h"
#include "headers/location.h"
extern int    network_message_type_message_delta;

extern void object_detach(int child_object_index);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern int actor_aim_grenade(int actor_index, const real_point3d *origin, real_vector3d *vector);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern float normalize3d(real_vector3d *v);
extern void projectile_accelerate(int projectile_index, const real_vector3d *acceleration);
extern uint8_t object_force_inside_bsp(int object_index, int ignore_object_index, const real_point3d *known_good_point);
extern int16_t game_connection(void);
extern uint8_t object_being_deleted(int object_index);
extern void object_type_update_baseline(int object_index);
extern int projectile_new_to_network(int object_index, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern void object_delete(int object_index);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

void unit_throw_grenade_release(int unit_index, uint8_t premature)
{
    unit_datum *object_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_definition *object_definition = TAG_GET(unit_definition, object_data->definition_index);

    if ( (unsigned __int8)object_data->unit.grenade_throw_state != _grenade_throw_state_in_hand )
        return;

    int grenade_index = object_data->unit.grenade_object_index;
    if ( grenade_index == -1 )
    {
        object_data->unit.grenade_throw_state = _grenade_throw_state_released;
        return;
    }

    object_detach(grenade_index);

    real_vector3d launch_velocity;
    if ( object_data->unit.actor_index == -1 )  /* player-controlled */
    {
        if ( object_data->unit.player_index != -1 )
        {
            float throw_x = object_data->unit.aiming_vector.n[0];
            float throw_y = object_data->unit.aiming_vector.n[1];
            float throw_z = object_data->unit.aiming_vector.n[2];
            const real_vector3d *up = global_up3d;
            game_globals_player_information *player_info =
                (game_globals_player_information *)global_game_globals->player_information.address;

            /* right = up x throw_velocity (fall back to up if degenerate) */
            real_vector3d right;
            right.n[0] = (up->n[1] * throw_z) - (up->n[2] * throw_y);
            right.n[2] = (up->n[0] * throw_y) - (up->n[1] * throw_x);
            right.n[1] = (up->n[2] * throw_x) - (up->n[0] * throw_z);
            if ( normalize3d(&right) == 0.0 )
                right = *up;

            /* basis = right x throw_velocity */
            real_vector3d basis;
            basis.n[0] = (right.n[2] * throw_y) - (right.n[1] * throw_z);
            basis.n[1] = (throw_z * right.n[0]) - (right.n[2] * throw_x);
            basis.n[2] = (right.n[1] * throw_x) - (right.n[0] * throw_y);
            normalize3d(&basis);

            /* release position = camera + throw offsets along (throw, right, basis) */
            real_point3d release_position;
            unit_get_camera_position(unit_index, &release_position);
            release_position.n[0] = (throw_x * player_info->grenade_origin.n[0]) + release_position.n[0];
            release_position.n[1] = (throw_y * player_info->grenade_origin.n[0]) + release_position.n[1];
            release_position.n[2] = (throw_z * player_info->grenade_origin.n[0]) + release_position.n[2];
            release_position.n[0] = (player_info->grenade_origin.n[1] * right.n[0]) + release_position.n[0];
            release_position.n[1] = (right.n[1] * player_info->grenade_origin.n[1]) + release_position.n[1];
            release_position.n[2] = (right.n[2] * player_info->grenade_origin.n[1]) + release_position.n[2];
            release_position.n[0] = (player_info->grenade_origin.n[2] * basis.n[0]) + release_position.n[0];
            release_position.n[1] = (basis.n[1] * player_info->grenade_origin.n[2]) + release_position.n[1];
            release_position.n[2] = (basis.n[2] * player_info->grenade_origin.n[2]) + release_position.n[2];
            object_translate(grenade_index, &release_position, 0);
        }

        float velocity_scale = object_definition->unit.grenade_velocity * SECONDS_PER_TICK;
        launch_velocity.n[0] = object_data->unit.aiming_vector.n[0] * velocity_scale;
        launch_velocity.n[1] = object_data->unit.aiming_vector.n[1] * velocity_scale;
        launch_velocity.n[2] = object_data->unit.aiming_vector.n[2] * velocity_scale;
    }
    else  /* AI-controlled */
    {
        real_point3d grenade_origin;
        object_get_origin(grenade_index, &grenade_origin);
        actor_aim_grenade(object_data->unit.actor_index, &grenade_origin, &launch_velocity);
    }

    if ( premature )
    {
        float throw_fraction = (float)object_data->unit.grenade_throw_ticks
                             / (float)object_data->unit.grenade_throw_full_power_ticks;
        if ( throw_fraction < 1.0f )
        {
            float weak = real_seed_random_range(get_global_random_seed_address(), 0.020000001, 0.046666667);
            launch_velocity.n[0] = (object_data->unit.aiming_vector.n[0] * weak) * (1.0f - throw_fraction)
                                 + (launch_velocity.n[0] * throw_fraction);
            launch_velocity.n[1] = (object_data->unit.aiming_vector.n[1] * weak) * (1.0f - throw_fraction)
                                 + (launch_velocity.n[1] * throw_fraction);
            launch_velocity.n[2] = (object_data->unit.aiming_vector.n[2] * weak) * (1.0f - throw_fraction)
                                 + (launch_velocity.n[2] * throw_fraction);
        }
    }

    /* convert to a velocity delta relative to the grenade's current velocity */
    object_datum *grenade_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, grenade_index)->datum;
    launch_velocity.n[0] -= grenade_data->object.translational_velocity.n[0];
    launch_velocity.n[1] -= grenade_data->object.translational_velocity.n[1];
    launch_velocity.n[2] -= grenade_data->object.translational_velocity.n[2];
    projectile_accelerate(grenade_index, &launch_velocity);

    object_data->unit.grenade_throw_state = _grenade_throw_state_released;
    object_data->unit.grenade_object_index = -1;

    real_point3d camera_position;
    unit_get_camera_position(unit_index, &camera_position);
    if ( !object_force_inside_bsp(grenade_index, -1, &camera_position) )
    {
        object_delete(grenade_index);
        return;
    }

    if ( !object_data->object.datum_role && game_connection() == _game_connection_network_server && !object_being_deleted(grenade_index) )
    {
        grenade_data->object.datum_role = _networked_datum_master;
        object_type_update_baseline(grenade_index);
        int length = projectile_new_to_network(grenade_index, g_message_encode_buffer, 32760);
        if ( length > 0 )
        {
            void *server = global_network_game_server_get();
            network_game_server_send_message_to_all_loaded_machines(
                server, network_message_type_message_delta, g_message_encode_buffer, length, 1, 0, 0, 3);
        }
    }
}
