#include "../../headers/ws/snd/BUFFER.h"
#include "../../headers/ws/snd/SYSTEM.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// 0x82697150 (snd::BUFFER::Play, D:\Projects\code\common\src.sys\drv\sound\snd.cpp:91).
// Base implementation of the virtual Play() slot. Two paths:
//   * If a positive start delay is armed (delay > 0) and the pending-delay bit (BST_DELAY,
//     0x100) is not already set, latch delayTimer = delay, set BST_DELAY, and push this buffer
//     onto snd::System->needToUpdate so the driver starts it once the delay elapses.
//   * Otherwise start immediately by clearing the pending-delay bit.
// Asserts the buffer is not currently in the stopped state (BST_STOPPED, 0x8).
void snd::BUFFER::Play()
{
    if (!IGNORE_STRONG_ASSERT && (state.val & BST_STOPPED) != 0)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("!IsStopped()",
                                                "D:\\Projects\\code\\common\\src.sys\\drv\\sound\\snd.cpp",
                                                91,
                                                empty_string);

    if ((state.val & BST_DELAY) != 0 || delay <= 0.0f)
    {
        // Already pending, or no delay requested: start now.
        state.val &= ~BST_DELAY;
    }
    else
    {
        // Arm the delayed-start: latch the timer and enqueue for the driver's update pass.
        state.val |= BST_DELAY;
        delayTimer = delay;

        snd::BUFFER *self = this;
        snd::System->needToUpdate.PushBack(self);
    }
}
