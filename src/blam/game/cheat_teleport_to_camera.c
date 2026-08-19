/* cheat_teleport_to_camera @0x8369A600 — teleport the acting player's unit to the current debug/observer
 * camera position. If the camera is outside the BSP (no cluster), prints a warning and does nothing. If the
 * unit is mounted in/attached to another object (object +0x11C parent != -1), the parent is teleported
 * instead so the whole composite moves. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/real_argb_color.h"
#include "headers/observer_result.h"
#include "headers/real_point3d.h"
#include "headers/player_datum.h"
#include "headers/object_datum.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern int cheat_player_index(void);
extern const observer_result *observer_get_camera(int16_t local_player_index);
extern void object_set_position(int object_index, const real_point3d *position, const real_vector3d *forward, const real_vector3d *up);
extern void terminal_printf(const real_argb_color *color, const char *format, ...);

void cheat_teleport_to_camera(void)
{
    int player_index = cheat_player_index();
    if ( player_index == -1 )
        return;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int observer_index = (uint16_t)player->local_player_index;
    if ( observer_index == 0xFFFF )
        return;

    const observer_result *camera = observer_get_camera(observer_index);
    if ( (uint16_t)camera->location.cluster_index == 0xFFFF )
    {
        terminal_printf(global_real_argb_orange, "Camera is outside BSP... cannot initiate teleportation...");
        return;
    }

    int object_index = player->unit_index;
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    if ( object_data->object.parent_object_index != -1 )
        object_index = object_data->object.parent_object_index;

    object_set_position(object_index, &camera->position, nullptr, nullptr);
}
