#include "STATE_FACTORY.h"

// sml::STATE_FACTORY::GetState @ 0x82666568
// Look up the registered name for a state id (bounds-checked dsVECTOR::operator[] const,
// boundary — not reversed in this batch).
const dsTSTRING<char> &sml::STATE_FACTORY::GetState(int idx) const
{
    return this->states[idx];
}
