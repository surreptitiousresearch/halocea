/* hcex_get_light_params @0x83683388 — resolve one dynamic light's placement for the HCEX bridge: fetch its
 * intensity (radius) from the driving object function, and if the light is active build a world matrix from
 * its position + forward/up basis. A light whose parent_light_index != -1 is suppressed (radius forced to 0).
 * Reports, via *plr_idx, which local player (1 then 0) controls the object the light is attached to (-1 if
 * none). When the light's tag is a first-person effect (tag flag 0x10) the basis is replaced with the
 * centered-flashlight transform; otherwise, for a held first-person weapon, it is adjusted to the light's
 * attachment marker before composing the matrix.
 *
 * NOTE: the DB names the 2nd parameter hcex_matr4x3*, which the decompiler (and the
 * matrix4x3_from_point_and_vectors call) treat as real_matrix4x3*. */

#include <stdint.h>
#include "../headers/data_array.h"
#include "../headers/player_datum.h"
#include "../headers/object_datum.h"
#include "../headers/light_datum.h"
#include "../headers/real_matrix4x3.h"
#include "../headers/real_point3d.h"
#include "../headers/real_vector3d.h"
#include "../headers/global_tag_instances.h"
#include "../headers/object_type.h"
#include "../headers/light_definition_flags.h"

extern "C" data_array *light_data;
extern "C" data_array *player_data;

extern "C" uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern "C" int local_player_get_player_index(int16_t local_player_index);
extern "C" void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern "C" const char *object_get_attachment_marker_name(int object_index, int16_t attachment_index);
extern "C" void first_person_weapon_center_flashlight(int unit_index, real_point3d *position, real_vector3d *forward, real_vector3d *up);
extern "C" uint8_t first_person_weapon_adjust_light(int weapon_index, const char *marker_name, real_point3d *position, real_vector3d *forward, real_vector3d *up);
extern "C" void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);

extern "C" void hcex_get_light_params(int lightId, real_matrix4x3 *matr, float *radius, int *plr_idx)
{
    light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, lightId);

    if ( light->parent_light_index != -1 )
    {
        *radius = 0.0f;
        return;
    }

    object_get_function_value(light->object_index, light->function_index, radius);
    if ( *radius < 0.000099999997f )
        return;

    int local_player;
    for ( local_player = 1; local_player >= 0; --local_player )
    {
        int controlled_unit;
        if ( local_player_get_player_index(local_player) == -1 )
            controlled_unit = -1;
        else
            controlled_unit = DATA_ARRAY_ELEMENT(player_data, player_datum,
                local_player_get_player_index(local_player))->unit_index;
        if ( controlled_unit == light->object_index )
            break;
    }
    *plr_idx = local_player;

    real_point3d position = light->position;
    real_vector3d forward = light->forward;
    real_vector3d up = light->up;

    object_datum *object = (object_datum *)object_try_and_get_and_verify_type(light->object_index, object_mask_all);

    if ( (**(int **)TAG_INSTANCE(light->definition_index) & (1u << _light_is_first_person_flashlight_bit)) != 0 )
    {
        first_person_weapon_center_flashlight(light->object_index, &position, &forward, &up);
    }
    else if ( object->object.type == object_type_weapon && object->object.parent_object_index != -1 )
    {
        const char *marker_name = object_get_attachment_marker_name(light->object_index,
            light->attachment_marker_index);
        first_person_weapon_adjust_light(light->object_index, marker_name, &position, &forward, &up);
    }

    matrix4x3_from_point_and_vectors(matr, &position, &forward, &up);
}
