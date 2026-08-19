/* motion_sensor_tick @0x837BD730 */
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"
#include <stdint.h>
/* motion_sensor_tick 0x837BD740 — advance the motion-sensor (radar) sweep each frame, then update the sensor.
 * The sweep position cycles with a 2.1-second period taken from the game clock (30 ticks/s, so ticks * 1/30
 * seconds). Near the end of a cycle (>= 2.0375 s) the sweep angle snaps to 0.4; otherwise it is the reciprocal
 * of (sweep_time + 0.0625) scaled by the calibration scalar test_0.
 * DEVIATION: no return value — r3 at both blr sites is only the tail-position motion_sensor_update() result
 * (0x837BD7AC / 0x837BD7D0) and the sole caller (hud_draw_screen @0x83708E78) ignores it; the decompiler had
 * threaded that callee status through as a phantom int return. */

extern int game_time_get(void);
extern double fmod(double x, double y);
extern void motion_sensor_update(void);


void motion_sensor_tick(void)
{
    float sweep_time = (float)fmod((float)game_time_get() * SECONDS_PER_TICK, 2.1f);
    if ( sweep_time >= 2.0374999f )
        sweep_theta = 0.40000001f;
    else
        sweep_theta = 1.0f / ((sweep_time + 0.0625f) * test_0);
    motion_sensor_update();
}
