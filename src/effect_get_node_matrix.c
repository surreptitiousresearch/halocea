/* effect_get_node_matrix @0x836E1F90 — resolve the world transform of a node designator for a live
 * effect. A designator of -1 means "no node" (returns the object's origin node, index -1). Negative
 * designators other than -1 carry the first-person bit (0x8000): the low 15 bits index a node on the
 * effect's first-person weapon. Otherwise the low 15 bits index a node on the effect's object. */

#include <stdint.h>
#include "headers/effect_datum.h"
#include "headers/real_matrix4x3.h"

extern real_matrix4x3 *first_person_weapon_get_node_matrix(int16_t local_player_index, int16_t node_index);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);

real_matrix4x3 * effect_get_node_matrix(effect_datum *effect, int16_t node_designator)
{
    __int16 node_index;

    if ( node_designator == -1 )
        return object_get_node_matrix(effect->object_index, -1);

    if ( node_designator < 0 )
        return first_person_weapon_get_node_matrix(effect->local_player_index, node_designator & 0x7FFF);

    node_index = node_designator & 0x7FFF;
    return object_get_node_matrix(effect->object_index, node_index);
}
