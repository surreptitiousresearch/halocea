/* scripted_camera_set_absolute @0x83705038 — point the scripted camera at a cutscene camera point in world
 * space (not anchored to any object). */

#include <stdint.h>

extern void scripted_camera_set(int16_t camera_point_index, int16_t tick_count, int relative_to_object_index);

void scripted_camera_set_absolute(int16_t camera_point_index, int16_t tick_count)
{
    scripted_camera_set(camera_point_index, tick_count, -1);
}
