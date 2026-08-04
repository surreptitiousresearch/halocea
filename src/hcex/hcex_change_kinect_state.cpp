/* hcex_change_kinect_state @0x823D1650 — per-frame HCEX bridge pump for the ws-engine Kinect
 * subsystem's deferred load/unload request: if gsInpKinect exists and its state has bit 0x1
 * (load pending) set, call Load and clear the bit; if bit 0x2 (unload pending) is set, call
 * Unload and clear it. gsINP_KINECT is a ws-engine boundary — only Load/Unload/state are
 * modeled (see hcex_kinect_boundary.h). */

#include "../headers/hcex/hcex_kinect_boundary.h"

void hcex_change_kinect_state(void)
{
    gsINP_KINECT *kinect = gsInpKinect;
    if ( !kinect )
        return;

    if ( (kinect->state.state & 1) != 0 )
    {
        gsINP_KINECT_Load(kinect);
        gsInpKinect->state.state &= ~1u;
        kinect = gsInpKinect;
    }

    if ( (kinect->state.state & 2) != 0 )
    {
        gsINP_KINECT_Unload(kinect);
        gsInpKinect->state.state &= ~2u;
    }
}
