#include "STATE_DESC.h"

// sml::STATE_DESC::Find @ 0x82666638
// Look up nameState among the entries recorded in this STATE_DESC by resolving each
// recorded id through the shared name registry and comparing names; returns the position
// within nameIdx, or -1 if not found here.
int sml::STATE_DESC::Find(const dsTSTRING<char> &nameState)
{
    for (int index = 0; index < this->nameIdx.nElem; ++index)
    {
        int stateId = this->nameIdx[index];
        const dsTSTRING<char> &registeredName = sml::fctState.GetState(stateId);
        if (registeredName == nameState)
            return index;
    }
    return -1;
}
