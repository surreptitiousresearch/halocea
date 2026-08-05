/* local_player_compute_autoaim_level_and_target_object_index_only @0x836C68F0 — run aim assist for a local
 * player and return only the chosen target object index and autoaim level (no magnetism application). Only
 * valid in first/third-person camera perspectives (0 or 1). Resolves the player's aiming unit and zoom
 * level, fetches its aim-assist parameters, then runs aim_assist from the camera position/forward, ignoring
 * the player's own unit and team. Returns the target object index (or -1 on any failure) and writes the
 * autoaim level. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/observer_result.h"
#include "headers/aim_assist_parameters.h"
#include "headers/aim_assist_target.h"
#include "headers/player_datum.h"
#include "headers/director_perspective.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern int16_t director_get_perspective(int16_t local_player_index);
extern int local_player_get_player_index(int16_t local_player_index);
extern int unit_get_aiming_unit_index(int unit_index);
extern int16_t player_control_get_zoom_level(int16_t local_player_index);
extern uint8_t unit_get_aim_assist_parameters(int unit_index, int16_t zoom_level, aim_assist_parameters *parameters);
extern const observer_result *observer_get_camera(int16_t local_player_index);
extern uint8_t aim_assist(const aim_assist_parameters *parameters, const real_point3d *position, const real_vector3d *direction, int ignore_object_index, int16_t ignore_team_index, aim_assist_target *target);

int local_player_compute_autoaim_level_and_target_object_index_only(int16_t local_player_index, float *autoaim_level)
{
    int16_t perspective = director_get_perspective(local_player_index);
    *autoaim_level = 0.0f;
    if ( perspective != _director_perspective_first_person && perspective != _director_perspective_third_person )
        return -1;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum,
        local_player_get_player_index(local_player_index));
    int aiming_unit_index = unit_get_aiming_unit_index(player->unit_index);
    int16_t zoom_level = player_control_get_zoom_level(local_player_index);

    aim_assist_parameters parameters;
    if ( !unit_get_aim_assist_parameters(aiming_unit_index, zoom_level, &parameters) )
        return -1;

    const observer_result *camera = observer_get_camera(local_player_index);
    aim_assist_target target;
    if ( !aim_assist(&parameters, &camera->position, &camera->forward, aiming_unit_index, player->team_index, &target) )
        return -1;

    *autoaim_level = target.autoaim_level;
    return target.object_index;
}
