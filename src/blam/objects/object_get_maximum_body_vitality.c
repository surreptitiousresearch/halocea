/* object_get_maximum_body_vitality @0x836B12A0 — the object's maximum body (health) vitality,
 * scaled by the per-team difficulty body multiplier unless ignore_difficulty is set.
 *
 * Deviation: the decompiler's `*((float *)&v4 + 1)` is the high word of the double register holding the
 * float result — it just returns the float value. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/game_difficulty_value.h"
#include "headers/blam_data_globals.h"

extern float game_difficulty_get_team_value(int16_t value_type, int16_t team_index);

float object_get_maximum_body_vitality(int object_index, uint8_t ignore_difficulty)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    float maximum = object->object.maximum_body_vitality;
    if ( ignore_difficulty )
        return maximum;
    return game_difficulty_get_team_value(_game_difficulty_enemy_vitality_scale, object->object.owner_team_index) * maximum;
}
