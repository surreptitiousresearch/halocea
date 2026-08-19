/* input_frame_begin @ 0x836FB868 — per-gamepad, accumulate per-button hold durations
 * (in ticks, saturating at 255) for the 8 primary and 8 alt digital buttons, latch the
 * four thumbstick axes, and apply dead zones.
 *
 * Reconciled against DB-verified gamepad_state (input_gamepad): the hold-tick accumulators
 * write into buttons[0..7] (primary) and buttons[8..15] (alt); the latched sticks are
 * sticks[0] (left) and sticks[1] (right) at 0x20. */

#include "headers/input_gamepad.h"
#include "headers/blam_data_globals.h"

#include "headers/point2d.h"
extern void apply_dead_zones(point2d *stick);

void input_frame_begin(void)
{
    int i;
    for ( i = 0; i < 4; ++i )
    {
        input_gamepad *gp = &input_globals.gamepads[i];
        int b;

        for ( b = 0; b < 8; ++b )
        {
            int held = hcex_gamepad_buttons[0][16 * i + b]
                     ? gp->buttons[b] + 1 : 0;
            if ( held > 255 ) held = 255;
            gp->buttons[b] = (unsigned char)held;
        }
        for ( b = 0; b < 8; ++b )
        {
            int held = hcex_gamepad_buttons[0][16 * i + b + 8]
                     ? gp->buttons[b + 8] + 1 : 0;
            if ( held > 255 ) held = 255;
            gp->buttons[b + 8] = (unsigned char)held;
        }

        gp->sticks[0].n[0] = hcex_sThumbLX[i];
        gp->sticks[0].n[1] = hcex_sThumbLY[i];
        gp->sticks[1].n[0] = hcex_sThumbRX[i];
        gp->sticks[1].n[1] = hcex_sThumbRY[i];
        apply_dead_zones(&gp->sticks[0]);
        /* recovered: &input_globals.key_ticks[40*i-124] -> gamepads[i].sticks[1] (right stick) */
        apply_dead_zones(&gp->sticks[1]);
    }
}
