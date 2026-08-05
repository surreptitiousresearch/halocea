/* motion_sensor_blip_set_type_and_size @0x837BC968 — classify one motion-sensor blip: its type from
 * blip_type_get (friend/foe/vehicle/etc.), and its size from the unit definition's motion-sensor-blip-size
 * word (definition word[332], byte +664), clamped to 0 when out of the 0..2 range or when the unit is
 * invalid. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/motion_sensor_globals.h"
#include "headers/object_type.h"
#include "headers/blip_type.h"
#include "headers/blam_data_globals.h"


extern uint8_t blip_type_get(int object_index, int local_player_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags); /* DB: returns void* */

void motion_sensor_blip_set_type_and_size(motion_sensor_blip *blip, int unit_index, int local_player_index)
{
    blip->type = blip_type_get(unit_index, local_player_index);

    int16_t blip_size = 0;
    if ( unit_index != -1 && object_try_and_get_and_verify_type(unit_index, object_mask_unit) )
    {
        object_datum *unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        unit_definition *unit_def = TAG_GET(unit_definition, unit->definition_index);
        blip_size = unit_def->unit.blip_type;   /* unitdef+664; hud_blip_type */
        if ( (unsigned int)blip_size >= NUMBER_OF_HUD_BLIP_TYPES )
            blip_size = _hud_blip_type_medium;
    }
    blip->size = blip_size;
}
