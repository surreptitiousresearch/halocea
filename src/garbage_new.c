/* garbage_new @0x837EA090 — mark an object as garbage (about to be discarded/despawned): set its
 * "garbage" object-header flag, raise the object flags _object_shadowless_bit (18) and
 * _object_deleted_when_deactivated_bit (19), and seed a randomized despawn timer (300-600 ticks). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/garbage_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"

extern void object_set_garbage(int object_index, uint8_t garbage);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

uint8_t garbage_new(int garbage_index)
{
    garbage_datum *object = (garbage_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, garbage_index)->datum;

    object_set_garbage(garbage_index, 1u);
    object->object.flags |= (1u << _object_shadowless_bit) | (1u << _object_deleted_when_deactivated_bit);

    unsigned int *seed = get_global_random_seed_address();
    object->garbage.destroy_timer = seed_random_range(seed, 300, 600);
    return 1;
}
