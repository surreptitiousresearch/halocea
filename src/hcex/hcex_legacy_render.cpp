/* hcex_legacy_render @0x8368C768 — when a deferred legacy (Blam) render has been requested, render one Blam
 * frame and present it. Samples the current clock and the time deltas (since the last game tick and since the
 * last legacy render), and on the render path updates main_globals.last_render_clocks so the next delta is
 * measured from here. main_game_render / main_present_frame are the Blam standalone-loop render entry points. */

#include <stdint.h>
#include "../headers/main_globals.h"

extern "C" int   hcex_do_legacy_render;
extern "C" int64_t system_clocks(void);
extern "C" float game_time_get_since_tick(void);
extern "C" double system_clock_delta_seconds_real(int64_t a, int64_t b);
extern "C" void  main_game_render(float time_delta_since_tick_sec, float time_delta_since_last_render_sec);
extern "C" void main_present_frame(void);

extern "C" void hcex_legacy_render(void)
{
    /* DEVIATION: the decompiler modeled the 64-bit system_clocks() result as an OVERLAPPED register pair
     * (>>32); the disasm keeps the full 64-bit value in r3 and later stores it whole to last_render_clocks. */
    int64_t current_clocks = system_clocks();
    float time_delta_since_tick_sec = game_time_get_since_tick();
    float time_delta_since_last_render_sec =
        system_clock_delta_seconds_real(current_clocks, main_globals.last_render_clocks);

    if ( hcex_do_legacy_render )
    {
        hcex_do_legacy_render = 0;
        main_game_render(time_delta_since_tick_sec, time_delta_since_last_render_sec);
        main_globals.last_render_clocks = current_clocks;
        main_present_frame();
    }
}
