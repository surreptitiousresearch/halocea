/* effect_move_to_next @0x836E2450 — debug fly-camera cheat: jump the free-fly camera to the next live effect
 * whose tag name contains `eff_name` and that has at least one resolved location instance. Starting just past the
 * previously-visited effect (last_effect_index), it scans the effect data array for a name match (skipping
 * effects flagged 0x8), wrapping to the start of the array if it runs off the end; for each candidate it asks
 * effect_location_get_next_instance for a location, records the effect as the new last_effect_index, and on the
 * first instance found resolves that instance's world position (the marker matrix position, transformed through
 * the referenced first-person-weapon or object node when a node is designated), backs the camera off along the
 * global forward vector by cam_ofs, and moves the fly camera there.
 *
 * DEVIATION: director_move_fly_camera takes three 12-byte structs BY VALUE, which the decompiler rendered as
 * punned pointer/__ROL4__ args; disasm (0x836E264C-836E26B4) settles it to (position, *global_forward3d,
 * *global_up3d). The transformed point is inst->matrix.position (r28+0x30). The two `(__int16)node_designator ==
 * -1` tests the decompiler keeps are dead (node_designator == 0xFFFF is handled first), so they are folded out. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/effect_datum.h"
#include "headers/effect_node_designator.h"
#include "headers/effect_location_datum.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include <string.h>
#include "headers/effect_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern void *datum_get(data_array *array, int index);
extern int data_next_index(const data_array *data, int16_t index);

extern char *tag_get_name(int16_t tag_index);
extern effect_location_datum *effect_location_get_next_instance(const effect_datum *effect, int *location_datum_index, int16_t camera_mode);
extern real_matrix4x3 *first_person_weapon_get_node_matrix(int16_t local_player_index, int16_t node_index);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern void director_move_fly_camera(real_point3d position, real_vector3d forward, real_vector3d up);

void effect_move_to_next(const char *eff_name)
{
    int effect_index;
    int index = data_next_index(effect_data, -1);
    if ( index != -1 )
    {
        while ( index != last_effect_index )
        {
            index = data_next_index(effect_data, index);
            if ( index == -1 )
                goto wrap;
        }
        effect_index = data_next_index(effect_data, index);
        if ( effect_index != -1 )
        {
            while ( 1 )
            {
                /* recovered: effect_data->data + 252*(u16)effect_index -> datum_get (252 == effect datum_size incl. 2-byte salt header) */
                effect_datum *effect = datum_get(effect_data, effect_index);
                char *name = tag_get_name(effect->definition_index);
                if ( (effect->flags & (1u << _effect_stopped_bit)) == 0 && name && strstr(name, eff_name) )
                    break;
                effect_index = data_next_index(effect_data, effect_index);
                if ( effect_index == -1 )
                    goto wrap;
            }
            goto scan;
        }
    }

wrap:
    effect_index = data_next_index(effect_data, -1);
    if ( effect_index == -1 )
        return;

scan:;
    effect_datum *effect;
    effect_location_datum *instance;
    while ( 1 )
    {
        /* recovered: effect_data->data + 252*(u16)effect_index -> datum_get (252 == effect datum_size incl. 2-byte salt header) */
        effect = datum_get(effect_data, effect_index);
        char *name = tag_get_name(effect->definition_index);
        if ( name && strstr(name, eff_name) && (effect->flags & (1u << _effect_stopped_bit)) == 0 )
        {
            int location_index = effect->location_datum_indices[0];
            last_effect_index = effect_index;
            instance = effect_location_get_next_instance(effect, &location_index, 0);
            if ( instance )
                break;
        }
        effect_index = data_next_index(effect_data, effect_index);
        if ( effect_index == -1 )
            return;
    }

    real_point3d position;
    unsigned __int16 node_designator = instance->node_designator;
    if ( node_designator == 0xFFFF )
    {
        position = instance->matrix.position;
    }
    else if ( EFFECT_NODE_IS_FIRST_PERSON_WEAPON(node_designator) )
    {
        real_matrix4x3 *node_matrix = first_person_weapon_get_node_matrix(effect->local_player_index,
                EFFECT_NODE_DESIGNATOR_TO_INDEX(node_designator));
        matrix4x3_transform_point(node_matrix, &instance->matrix.position, &position);
    }
    else
    {
        real_matrix4x3 *node_matrix = object_get_node_matrix(effect->object_index, EFFECT_NODE_DESIGNATOR_TO_INDEX(node_designator));
        matrix4x3_transform_point(node_matrix, &instance->matrix.position, &position);
    }

    position.n[0] -= global_forward3d->n[0] * cam_ofs;
    position.n[1] -= global_forward3d->n[1] * cam_ofs;
    position.n[2] -= global_forward3d->n[2] * cam_ofs;
    director_move_fly_camera(position, *global_forward3d, *global_up3d);
}
