/* dead_camera_new @ 0x837C3168 — initialize the death/respawn camera. It starts at the current observer
 * camera position and picks a random orbit (distance 2..6, random yaw, downward pitch) about the dead
 * player. The 3-second timer governs the death-cam hold; switch_timer controls when to switch to
 * spectating another player: infinite when a specific unit was supplied, else 15s in a running game
 * engine (multiplayer) or 3s otherwise. When no unit is given, it targets the player's own last unit. */

#include <stdint.h>
#include "headers/dead_camera.h"
#include "headers/player_datum.h"
#include "headers/observer_result.h"
#include "headers/data_array.h"
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"


extern const observer_result *observer_get_camera(int16_t local_player_index);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern uint8_t game_engine_running(void);
extern int local_player_get_player_index(int16_t local_player_index);

void dead_camera_new(dead_camera *camera, int16_t local_player_index, int unit_index)
{
    const observer_result *observer = observer_get_camera(local_player_index);
    int player_index;

    camera->position.n[0] = observer->position.n[0];
    camera->position.n[1] = observer->position.n[1];
    camera->position.n[2] = observer->position.n[2];
    camera->field_of_view = 1.2217305f;

    camera->distance = real_seed_random_range(get_global_local_random_seed_address(), 2.0, 6.0);
    camera->orientation.n[0] = real_seed_random_range(get_global_local_random_seed_address(), 0.0, TWO_PI);
    camera->orientation.n[1] = -real_seed_random_range(get_global_local_random_seed_address(), 0.47123894, 1.0995574);
    camera->timer = 3.0f;

    if ( unit_index == -1 )
        camera->switch_timer = game_engine_running() ? 15.0f : 3.0f;
    else
        camera->switch_timer = 3.4028235e38f;   /* effectively infinite */

    player_index = local_player_get_player_index(local_player_index);
    camera->player_index = player_index;
    camera->current_player_index = player_index;

    if ( unit_index == -1 )
    {
        /* target the player's own last unit (player datum +56) */
        camera->unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->dead_unit_index;
    }
    else
    {
        camera->unit_index = unit_index;
    }
}
