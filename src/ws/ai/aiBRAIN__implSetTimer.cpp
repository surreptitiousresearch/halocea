#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"

// aiBRAIN::implSetTimer @ 0x8317AC00
// ?implSetTimer@aiBRAIN@@UAAHM_N@Z
//
// Arm a brain timer of `duration` seconds, tagged with the current best-behaviour context; cyclic
// when `isCycle`. Returns the assigned timer id. Mirrors aiTIMER_POOL::AddTimer, but resolves the
// context from the mind's best-behaviour module instead of taking it as an argument.
//
// DEVIATION: the mangled signature is (float duration, bool isCycle) -> int. The decompiler
// invented an extra bool slot (PPC ABI artifact) and read the single bool twice; corrected here.
int aiBRAIN::implSetTimer(float duration, bool isCycle)
{
    aiTIMER_POOL *pool = &this->timers;
    int context = this->mind.best.pointee->GetBhvContext();

    int newID = pool->lastID + 1;
    pool->lastID = newID;
    if (newID < 0)
        pool->lastID = 0;
    int id = pool->lastID;

    aiTIMER_POOL::aiTIMER entry;
    entry.timer                  = duration;
    entry.aiTIMER_DATA::context  = context;
    entry.defVal                 = duration;
    entry.isCycle                = isCycle;
    entry.aiTIMER_DATA::id       = id;
    pool->timers.PushBack(entry);

    return pool->lastID;
}
