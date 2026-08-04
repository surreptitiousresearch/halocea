#include "mtlMTL_MNG.h"
#include "../ds/dsAFFIX_BUILDER.h"

// mtlMTL_MNG::FilterAffixStringOverride(const dsTSTRING<char> &) const — 0x82A86ED0
// Parse affixString into a dsAFFIX_BUILDER, keep only the affixes whose name contains the
// substring "vid.sdr.", and rebuild the filtered set back into a single string.
// ABI: r3 = sret result buffer, r4 = this (loaded, verified unused by disasm — overwritten by
// "mr r4, r5" before ever being read), r5 = affixString. The final BuildString() call writes its
// result directly through this function's own sret buffer (r3 passed straight through).
dsTSTRING<char> mtlMTL_MNG::FilterAffixStringOverride(const dsTSTRING<char> &affixString) const
{
    dsAFFIX_BUILDER parsedAffixes(affixString);
    dsAFFIX_BUILDER filteredAffixes;

    for (int affixIndex = 0; affixIndex < parsedAffixes.affixes.nElem; ++affixIndex)
    {
        const dsAFFIX_BUILDER::AFFIX &affix = parsedAffixes.affixes[affixIndex];
        bool nameHasVidSdrPrefix = affix.name.Find("vid.sdr.", 0, 0) != -1;
        if (nameHasVidSdrPrefix)
            filteredAffixes.AddAffixFrom(parsedAffixes, affixIndex);
    }

    return filteredAffixes.BuildString();
}
