/* cheat_spawn_warthog @0x8369A360 — locate the "warthog" entry in the multiplayer vehicle list (by name
 * substring) and spawn it just in front of the acting player, oriented to match the player's facing.
 *
 * The original contains a single-iteration spawn loop (the fan-out machinery used by cheat_objects, here
 * fixed at one instance); reproduced faithfully. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_globals_multiplayer_information.h"
#include "headers/player_datum.h"
#include "headers/game_globals_definition.h"
#include "headers/game_globals_tag_reference.h"
#include "headers/object_placement_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern int cheat_player_index(void);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void object_get_orientation(int object_index, real_vector3d *forward, real_vector3d *up);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern char *strstr(const char *haystack, const char *needle);
extern double atan2(double y, double x);
extern double cos(double x);
extern double sin(double x);

void cheat_spawn_warthog(void)
{
    if ( !global_game_globals->multiplayer_information.count )
        return;

    const game_globals_tag_reference *vehicle_list =
        (const game_globals_tag_reference *)((game_globals_multiplayer_information *)global_game_globals->multiplayer_information.address)->vehicles.address;
    int vehicle_count = ((game_globals_multiplayer_information *)global_game_globals->multiplayer_information.address)->vehicles.count;
    if ( vehicle_count <= 0 )
        return;

    int warthog_index = 0;
    while ( !strstr(vehicle_list[warthog_index].reference.name, "warthog") )
    {
        if ( ++warthog_index >= vehicle_count )
            return;
    }

    int player_index = cheat_player_index();
    if ( player_index == -1 )
        return;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int unit_index = player->unit_index;

    real_point3d origin;
    real_vector3d forward, up;
    object_get_origin(unit_index, &origin);
    object_get_orientation(unit_index, &forward, &up);

    for ( int i = 0; i < 1; i = (int16_t)(i + 1) )
    {
        int tag_index = vehicle_list[warthog_index + i].reference.index;
        if ( tag_index == -1 )
            continue;

        float angle = ((float)i * 0.39269909f) + (float)atan2(forward.n[1], forward.n[0]);

        object_placement_data placement;
        object_placement_data_new(&placement, tag_index, -1);
        placement.position.n[0] = ((float)cos(angle) * 1.5f) + origin.n[0];
        placement.position.n[1] = ((float)sin(angle) * 1.5f) + origin.n[1];
        placement.position.n[2] = origin.n[2] + 0.80000001f;
        placement.forward = forward;
        placement.up = up;
        object_new(&placement);
    }
}
