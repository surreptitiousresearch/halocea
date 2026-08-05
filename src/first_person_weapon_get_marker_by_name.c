/* first_person_weapon_get_marker_by_name @0x8369EF48 — resolve named markers on a local player's
 * first-person weapon viewmodel, using the posed node matrices. Returns 0 unless the weapon is valid, is
 * actually shown in first person (perspective 0), and has a valid node remapping table.
 *
 * Weapon definition: interface first-person model, and the first-person animation graph whose
 * nodes.count sizes the marker lookup. */

#include <stdint.h>
#include <stddef.h>   /* NULL */
#include "headers/global_tag_instances.h"
#include "headers/first_person_weapon.h"
#include "headers/object_marker.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/animation_graph.h"
#include "headers/object_type.h"

#include "headers/real_matrix4x3.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int16_t first_person_weapon_index_from_weapon_index(int weapon_index);
extern int16_t director_get_perspective(int16_t local_player_index);
extern int16_t model_get_marker_by_name(int model_index, const char *name, const uint8_t *region_permutations, const int16_t *node_remapping_table, int16_t node_count, const real_matrix4x3 *node_matrices, uint8_t mirrored_flag, object_marker *markers, int16_t maximum_marker_count);

int16_t first_person_weapon_get_marker_by_name(int weapon_index, const char *name, object_marker *markers,
                                               int16_t maximum_marker_count)
{
    weapon_datum *weapon_object = object_try_and_get_and_verify_type(weapon_index, object_mask_weapon);
    int16_t person_index;
    first_person_weapon *fpw;
    weapon_definition *definition;
    int model_index;
    int animation_graph_index;

    if ( !weapon_object )
        return 0;

    person_index = first_person_weapon_index_from_weapon_index(weapon_index);
    if ( person_index == -1 )
        return 0;
    if ( director_get_perspective(person_index) )
        return 0;

    fpw = &first_person_weapons[person_index];
    definition = TAG_GET(weapon_definition, weapon_object->definition_index);
    if ( !fpw->weapon_node_remapping_table_valid )
        return 0;

    model_index = definition->weapon.interface_definition.first_person_model.index;
    if ( model_index == -1 )
        return 0;
    animation_graph_index = definition->weapon.interface_definition.first_person_animations.index;
    if ( animation_graph_index == -1 )
        return 0;

    return model_get_marker_by_name(
        model_index,
        name,
        NULL,
        fpw->weapon_node_remapping_table,
        TAG_GET(animation_graph, animation_graph_index)->nodes.count,
        fpw->node_matrices,
        0,
        markers,
        maximum_marker_count);
}
