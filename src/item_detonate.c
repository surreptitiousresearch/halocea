/* item_detonate @0x83757DC0 — detonate an item (e.g. a dropped grenade/explosive). If its detonation timer
 * (item.detonation_ticks) hasn't been armed, spawn the item definition's detonating_effect from the item
 * and arm the timer to a random duration in [detonation_delay_timer_lower_bound, _upper_bound] * 30 ticks.
 * DEVIATION: the decompiler passed the datum index as the color arg and a stray marker as the impulse field;
 * the disassembly sets both (r9/r10) to 0 — color and impulse_field are null. Soft-float reproduced as floats. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/item_datum.h"
#include "headers/item_definition.h"
#include "headers/blam_data_globals.h"

/* real_rgb_color comes from headers (union); forward typedef removed to avoid struct/union clash */
typedef struct effect_vector_field effect_vector_field;

#include "headers/real_rgb_color.h"
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

void item_detonate(int item_index)
{
    item_datum *item = (item_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, item_index)->datum;
    item_definition *definition = TAG_GET(item_definition, item->definition_index);

    if ( !item->item.detonation_ticks )
    {
        effect_new_from_object(definition->item.detonating_effect.index, item_index, item_index, -1,
                               0.0f, 0.0f, 0, 0);
        float upper_bound = definition->item.detonation_delay_timer_upper_bound;
        float lower_bound = definition->item.detonation_delay_timer_lower_bound;
        unsigned int *seed = get_global_random_seed_address();
        item->item.detonation_ticks =
            (int)(real_seed_random_range(seed, lower_bound, upper_bound) * 30.0f);
    }
}
