#include "../../headers/ws/strm/strmSTREAMING_QUEUE.h"
#include "../../headers/ws/strm/strm_boundary.h"

// strmSTREAMING_QUEUE::EnableStrmThread @ 0x828EFBF0
// Toggle the worker thread's run/idle signal pair and record the new `enabled` state. A
// redundant enable(true) while already enabled, or disable(false) while already disabled, is a
// no-op (the signals are left untouched).
void strmSTREAMING_QUEUE::EnableStrmThread(bool enable)
{
    if (enable) {
        if (enabled)
            return;
        osSetSignal(hEventStrmThreadOn);
        osResetSignal(hEventStrmThreadOff);
        enabled = true;
        return;
    }

    if (!enabled)
        return;

    osSetSignal(hEventStrmThreadOff);
    osResetSignal(hEventStrmThreadOn);
    enabled = false;
}
