/* scenery_animation_start @0x837EA080 — begin a scenery object's animation from frame 0.
 * Thin wrapper: forwards to scenery_animation_start_private with frame_index = 0. */

#include <stdint.h>

extern void scenery_animation_start_private(int scenery_index, int animation_graph_index, const char *animation_name, int16_t frame_index);

void scenery_animation_start(int scenery_index, int animation_graph_index, const char *animation_name)
{
    scenery_animation_start_private(scenery_index, animation_graph_index, animation_name, 0);
}
