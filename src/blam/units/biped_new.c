/* biped_new @0x837B0C38 — post-creation setup for a newly placed biped: resets its support/pathfinding
 * state (ground plane to the "depths of hell" sentinel, jump recovery timer maxed, surfaces/targets
 * cleared), samples its origin as the initial pathfinding point, optionally searches for nearby support
 * (definition biped.flags bit 0x40), snaps its facing to the definition's default, clears elevator riding
 * state, and — on a networked connection — resets its network baseline/message tracking and "at rest" flag
 * (object sub-block, opaque; was_network_at_rest @+5, force_shield_update @+286 within it, per DB
 * types_members). Always returns 1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/biped_definition_flags.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"


extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void biped_find_nearby_support_surface(int biped_index);
extern void biped_snap_facing(biped_datum *biped, const biped_definition *biped_definition);
extern int16_t game_connection(void);

uint8_t biped_new(int biped_index)
{
    biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);
    const biped_definition *definition = TAG_GET(const biped_definition, biped->definition_index);

    biped->biped.ground_plane = depths_of_hell;
    biped->biped.jump_recovery_timer = 127;
    biped->biped.support_surface_index = -1;
    biped->biped.pathfinding_surface_index = -1;
    object_get_origin(biped_index, &biped->biped.pathfinding_point);
    biped->biped.last_pathfinding_surface_index = -1;
    biped->biped.last_pathfinding_attempt_time = -1;
    biped->biped.impact_target_object_index = -1;

    if ( (definition->biped.flags & (1u << _biped_climbs_anything_bit)) != 0 )
        biped_find_nearby_support_surface(biped_index);

    biped_snap_facing(biped, definition);

    biped->biped.elevator_object_index = -1;
    biped->biped.elevator_ticks = 0;

    int connection = game_connection();
    if ( connection == _game_connection_network_client || connection == _game_connection_network_server )
    {
        biped->biped.baseline_valid = 0;
        biped->biped.baseline_index = 0;
        biped->biped.message_index = 0;
        biped->object.was_network_at_rest = 0;
    }

    biped->object.force_shield_update = 0;
    return 1;
}
