/* main_update_time @ 0x8368A878 — advance the frame clock: measure real elapsed time
 * since last frame, clamp it (and cap to the 15/30 Hz frame budget out of cinematics),
 * then store seconds_elapsed. In timeDemo mode a fixed 33ms step is used instead. */
#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/_LARGE_INTEGER.h"

/* Win32 externs instead of <windows.h> (which collides with Blam stub headers). */
extern void Sleep(unsigned long dwMilliseconds);
extern int  QueryPerformanceFrequency(_LARGE_INTEGER *lpFrequency);
#include "headers/blam_data_globals.h"

extern uint8_t cinematic_in_progress(void);
extern int64_t system_clocks(void);
extern double system_clock_delta_seconds_real(int64_t a, int64_t b);
extern uint32_t system_clocks_to_milliseconds(int64_t clocks);
extern void hcex_correct_time_elapsed(float *);

void main_update_time(void)
{
    int64_t now_clocks;
    /* DEVIATION: dt respelled float->double 2026-08-18; the callee returns a true double
       (fdiv @0x83762D3C, no frsp), fmr f31,f1 keeps it wide, every clamp compares lfd DOUBLE
       constants, and the sole narrowing is frsp @0x8368A9CC before the stfs. A float cap takes
       a different clamp branch near 1/15 (0.066666670f vs 0.066666666666666666). */
    double dt;

    if ( !timeDemo && !global_frame_rate_throttle )
        cinematic_in_progress();

    now_clocks = system_clocks();
    dt = system_clock_delta_seconds_real(main_globals.last_time_clocks, now_clocks);
    Sleep(0);

    if ( main_globals.movie )
    {
        dt = main_globals.recording_dt;
    }
    else
    {
        main_globals.did_time_overflow_occur = dt > 1.0;
        if ( dt < 0.0 )
            dt = 0.0;
        else if ( dt > 1.0 )
            dt = 1.0;
        if ( !main_globals.connection && !cinematic_in_progress() )
        {
            double cap = debug_force_frame_rate_update ? (1.0 / 30.0) : (1.0 / 15.0);
            if ( dt > cap )
                dt = cap;
        }
    }

    if ( timeDemo )
    {
        /* fixed 33ms-per-frame stepping for deterministic timedemo playback */
        _LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        main_globals.last_time_clocks += freq.QuadPart / 30;
        main_globals.last_time_msec += 33;
        main_globals.seconds_elapsed = 1.0f / 30.0f;
    }
    else
    {
        main_globals.last_time_clocks = now_clocks;
        main_globals.last_time_msec = system_clocks_to_milliseconds(now_clocks);
        main_globals.seconds_elapsed = dt;
        hcex_correct_time_elapsed(&main_globals.seconds_elapsed);
    }
}
