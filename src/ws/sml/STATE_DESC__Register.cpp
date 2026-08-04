#include "STATE_DESC.h"

// sml::STATE_DESC::Register @ 0x82666750
// Return this STATE_DESC's existing index for nameState if already recorded; otherwise
// register nameState with the shared name registry (or reuse its id there), append that
// id to nameIdx, and return the newly appended index.
int sml::STATE_DESC::Register(const dsTSTRING<char> &nameState)
{
    int existingIndex = this->Find(nameState);
    if (existingIndex != -1)
        return existingIndex;

    int stateId = sml::fctState.Register(nameState);
    this->nameIdx.PushBack(stateId);
    return this->nameIdx.nElem - 1;
}
