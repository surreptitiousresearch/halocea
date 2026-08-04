/* collision_model_get_material_type @0x83772A18 — look up a material's type field within the collision
 * model's resistance.materials tag_block (72-byte damage_material records, type at +0x24). */

#include <stdint.h>
#include "headers/collision_model_instance.h"
#include "headers/damage_material.h"

int collision_model_get_material_type(const collision_model *model, int16_t material_index)
{
    if ( material_index == -1 )
        return -1;
    const damage_material *material =
        &((const damage_material *)model->resistance.materials.address)[material_index];
    return material->type;
}
