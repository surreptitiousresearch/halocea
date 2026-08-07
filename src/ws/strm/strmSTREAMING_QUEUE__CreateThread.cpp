#include "../../headers/ws/strm/strmSTREAMING_QUEUE.h"
#include "../../headers/ws/strm/strm_boundary.h"

// strmSTREAMING_QUEUE::CreateThread @ 0x828F3860
// Lazily spawn (once) the "streaming" worker thread running ThreadProc(this) at priority 1.
void strmSTREAMING_QUEUE::CreateThread()
{
    if (threadExist)
        return;

    osHANDLE_DUMMY *thread = osCreateThread(
        &strmSTREAMING_QUEUE::ThreadProc, this, "streaming", -1);
    hThread = thread;
    osSetThreadPriority(thread, 1);
    threadExist = true;
}
