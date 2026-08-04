#include "STATE_FACTORY.h"

// sml::STATE_FACTORY::Find @ 0x82666598
// Linear scan for an existing state registered under `nameState`; returns its id
// (index into states) or -1 if not found.
int sml::STATE_FACTORY::Find(const dsTSTRING<char> &nameState)
{
    for (int stateIndex = 0; stateIndex < this->states.nElem; ++stateIndex) {
        if (this->states[stateIndex] == nameState)
            return stateIndex;
    }
    return -1;
}
