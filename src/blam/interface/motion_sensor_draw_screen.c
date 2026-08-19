/* motion_sensor_draw_screen @0x837BDBA0 */
#include <stdint.h>
#include "headers/point2d.h"

extern void update_motion_sensor(int16_t local_player_index);
extern void render_motion_sensor(int16_t local_player_index, uint8_t in_multiplayer, const point2d *reference);

void motion_sensor_draw_screen(int16_t local_player_index, uint8_t in_multiplayer, const point2d *pt)
{
    if (local_player_index != -1)
    {
        update_motion_sensor(local_player_index);
        render_motion_sensor(local_player_index, in_multiplayer, pt);
    }
}
