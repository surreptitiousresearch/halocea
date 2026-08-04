#include "dsAFFIX_BUILDER.h"

// dsAFFIX_BUILDER::AddAffixFrom @ 0x82A78758
// Copy the affix at idAffix out of another builder and append it to this one.
void dsAFFIX_BUILDER::AddAffixFrom(const dsAFFIX_BUILDER &ab, int idAffix)
{
    const AFFIX &affix = ab.affixes[idAffix];
    this->affixes.PushBack(affix);
}
