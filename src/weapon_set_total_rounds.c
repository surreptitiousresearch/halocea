/* weapon_set_total_rounds @0x836DA388 — set the loaded and carried round counts of every magazine of a weapon
 * object to the values in rounds_array (one entry per magazine), each clamped to that magazine's capacity. The
 * carried (unloaded) count is additionally clamped to never exceed the new loaded count.
 * (DB naming: magazines[].rounds_total is the reserve, rounds_loaded the clip.) */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/blam_data_globals.h"


void weapon_set_total_rounds(int weapon_index, int16_t *rounds_array)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);

    int magazine_count = definition->weapon.magazines.count;
    for (int i = 0; i < magazine_count; i = (int16_t)(i + 1))
    {
        weapon_magazine_definition *magazine_definition =
            &((weapon_magazine_definition *)definition->weapon.magazines.address)[i];
        int16_t magazine_capacity = magazine_definition->rounds_total_maximum;
        int16_t total = magazine_capacity;
        if (total > rounds_array[i])
            total = rounds_array[i];

        weapon_magazine *magazine = &weapon->weapon.magazines[i];
        magazine->rounds_total = total;
        int16_t loaded = magazine->rounds_loaded;
        if (loaded > total)
            loaded = total;
        magazine->rounds_loaded = loaded;
    }
}
