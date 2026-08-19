/* scenery_animation_start_at_frame @0x837EA088 — begin a scenery object's animation from a given frame.
 * Thin wrapper: forwards directly to scenery_animation_start_private (frame_index passes through). */

#include <stdint.h>

extern void scenery_animation_start_private(int scenery_index, int animation_graph_index, const char *animation_name, int16_t frame_index);

void scenery_animation_start_at_frame(int scenery_index, int animation_graph_index, const char *animation_name, int16_t frame_index)
{
    scenery_animation_start_private(scenery_index, animation_graph_index, animation_name, frame_index);
}
