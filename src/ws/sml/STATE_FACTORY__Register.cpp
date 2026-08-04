#include "STATE_FACTORY.h"

// sml::STATE_FACTORY::Register @ 0x826666E8
// Register a new named state, or return the id it is already registered under.
int sml::STATE_FACTORY::Register(const dsTSTRING<char> &nameState)
{
    int existingId = this->Find(nameState);
    if (existingId != -1)
        return existingId;

    this->states.PushBack(nameState);
    return this->states.nElem - 1;
}
