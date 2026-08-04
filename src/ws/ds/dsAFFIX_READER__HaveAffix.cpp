#include "dsAFFIX_READER.h"

// dsAFFIX_READER::HaveAffix @ 0x82A755A0
// Search for affix, cache the located sub-string index, and report whether it was found.
bool dsAFFIX_READER::HaveAffix(const char *affix, int asSubString)
{
    int foundIndex = this->FindAffix(affix, asSubString);
    this->curSubStr = foundIndex;
    return foundIndex != -1;
}
