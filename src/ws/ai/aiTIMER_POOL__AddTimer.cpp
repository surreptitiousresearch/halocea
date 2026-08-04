#include "../../headers/ws/ai/aiTIMER_POOL.h"

// aiTIMER_POOL::AddTimer @ 0x8317AAB0
// ?AddTimer@aiTIMER_POOL@@QAAHM_NH@Z
//
// Allocate a fresh timer id (rolling, wrapping to 0 on signed overflow), fill an aiTIMER record
// with the requested duration/cycle/context and append it to the pool.
//
// DEVIATION: the mangled signature is (float time, bool isCycle, int context) -> int. The
// decompiler split the int `context` into a phantom bool + int pair; corrected here to match the
// name-mangling. `time` also decompiled as `double` (PPC vararg-slot artifact) — it is a float.
int aiTIMER_POOL::AddTimer(float time, bool isCycle, int context)
{
    int newID = this->lastID + 1;
    this->lastID = newID;
    if (newID < 0)          // wrap on signed overflow
        this->lastID = 0;
    int id = this->lastID;

    aiTIMER entry;
    entry.timer                  = time;
    entry.defVal                 = time;
    entry.aiTIMER_DATA::context  = context;
    entry.isCycle                = isCycle;
    entry.aiTIMER_DATA::id       = id;
    this->timers.PushBack(entry);

    return this->lastID;
}
