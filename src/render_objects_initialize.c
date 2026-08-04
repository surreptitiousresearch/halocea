/* render_objects_initialize @ 0x836E5C30 — cached object render-state pool */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

/* void: r3 at blr is only game_state_data_new's residue (stored to the global, no
 * independent return); Blam _initialize convention. No sampled callers. */
void render_objects_initialize(void)
{
    cached_object_render_states = game_state_data_new("cached object render states", 256, 256);
}
