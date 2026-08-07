/* first_person_weapon_start_interpolation @0x8369F2A8 — begin interpolating the first-person viewmodel pose
 * over `frame_count` frames. Snapshots the current node orientations into the "original" buffer (the source
 * pose to blend from), copying only the node count declared by the weapon's first-person animation graph
 * (32 bytes per real_orientation). The new blend is only (re)armed when it is at least as long
 * as whatever interpolation is already in flight. */

#include "headers/first_person_weapon.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/animation_graph.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>
#include <string.h>


void first_person_weapon_start_interpolation(int16_t local_player_index, int16_t frame_count)
{
    first_person_weapon *fp_weapon = &first_person_weapons[local_player_index];

    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, fp_weapon->weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    animation_graph *graph = TAG_GET(animation_graph, definition->weapon.interface_definition.first_person_animations.index);
    int node_count = graph->nodes.count;

    memcpy(fp_weapon->original_node_orientations, fp_weapon->node_orientations, 32 * node_count);

    if ( frame_count >= fp_weapon->interpolation_frame_count - fp_weapon->interpolation_frame_index )
    {
        fp_weapon->interpolation_frame_count = frame_count;
        fp_weapon->interpolation_frame_index = 0;
    }
}
