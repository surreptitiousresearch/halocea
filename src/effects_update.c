/* effects_update @ 0x836E43D8 — tick every live effect in the effect data array. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void effect_update(int effect_index, float dt);

void effects_update(float dt)
{
    int i;
    for ( i = data_next_index(effect_data, -1); i != -1; i = data_next_index(effect_data, i) )
        effect_update(i, dt);
}
