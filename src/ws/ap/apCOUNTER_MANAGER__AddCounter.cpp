#include "apCOUNTER_MANAGER.h"
#include "apCOUNTER.h"
#include "apCOUNTER_TRACKER.h"
#include "../os/osLOCK.h"

// apCOUNTER_MANAGER::AddCounter @ 0x82718E60
// Register `counter` and return its index in the counter list. Thread-safe (holds `lock`).
// Deviation: the decompiler fabricated a3..a13 phantom parameters (float-slot-skip ABI artifact);
// the real signature takes a single apCOUNTER*. Its `a14 = counter` is just the by-value stack
// slot PushBack takes the address of.
int apCOUNTER_MANAGER::AddCounter(apCOUNTER *counter)
{
    apCOUNTER *counterSlot = counter;

    this->lock.Lock(nullptr, 0);

    this->IsDuplicatedCntName(&counter->name);
    if (this->cntTracker.started)
        this->cntTracker.AddLateCounter(counter);

    this->counters.PushBack(counterSlot);
    if (this->isFirstUpdate)
        this->countersToSend.PushBack(counterSlot);

    int newCount = this->counters.nElem;

    this->lock.Unlock(nullptr, 0);
    return newCount - 1;
}
