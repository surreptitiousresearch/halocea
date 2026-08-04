#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"

// aiFILT_ENEMY::HasAChoice @ 0x83220D68
// ?HasAChoice@aiFILT_ENEMY@@QAA_NAAPAVentENTITY@@@Z
//
// With 0 or 1 candidates the answer is forced: write the single entity (or null) into `out` and
// report false (no choice). With more than one candidate report true; `out` is left untouched and
// the caller must run the visibility selection.
bool aiFILT_ENEMY::HasAChoice(entENTITY *&out)
{
    const int nElem = arr.nElem;
    if (!nElem)
    {
        out = nullptr;
        return false;
    }
    if (nElem == 1)
    {
        aiFEE &fee = arr[0];
        out = fee.hold->watcher->GetEntity();
        return false;
    }
    return true;
}
