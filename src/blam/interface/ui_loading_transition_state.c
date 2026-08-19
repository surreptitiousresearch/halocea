#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/loading_screen_state.h"
/* ui_loading_transition_state @0x83764AA8 — request a loading-screen state transition: state 0 always
 * starts closing; otherwise the transition is only applied from states 1/2/4 (from 4 it also clears the
 * NAT-negotiation cookie), and from 2 only if the target state is 8. */

extern void ui_loading_start_closing(uint8_t why_are_we_always_preparing_just_go);

void ui_loading_transition_state(int state)
{
    char should_transition;

    if ( !state )
    {
        ui_loading_start_closing(1u);
        return;
    }

    if ( loading_screen_state != LOADING_STATE_SOLO )
    {
        if ( loading_screen_state != LOADING_STATE_SERVER )
        {
            if ( loading_screen_state != LOADING_STATE_NEGOTIATING )
            {
                should_transition = 1;
                goto apply;
            }
            loading_screen_natneg_cookie = -1;
        }
        should_transition = (state == LOADING_STATE_LOADING_MP_MAP);
        goto apply;
    }
    should_transition = 0;

apply:
    if ( should_transition )
        loading_screen_state = state;
}
