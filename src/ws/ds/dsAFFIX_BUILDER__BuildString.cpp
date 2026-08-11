// FUNCTION_INDEX entry: dsAFFIX_BUILDER::BuildString() const @0x82A79510 (?BuildString@dsAFFIX_BUILDER@@QBA?AV?$dsTSTRING@D@@XZ)
#include "dsAFFIX_BUILDER.h"
#include "dsVECTOR.h"
#include "dsTSTRING.h"

// CAVEAT: sret/this swap. The DB prototype is
//   ?BuildString@dsAFFIX_BUILDER@@QBA?AV?$dsTSTRING@D@@XZ
// i.e. `dsTSTRING<char> dsAFFIX_BUILDER::BuildString() const`, returned by value. The decompiler
// rendered it as `BuildString(dsAFFIX_BUILDER *this, dsTSTRING<char> *result)` where its `this` is
// actually the hidden sret return-buffer (a dsTSTRING<char>*) and its `result` is the real const
// object (a dsAFFIX_BUILDER*). This reconstruction restores the true signature: `this` is the
// dsAFFIX_BUILDER, and the returned string is a local. The empty-string singleton setup that the
// decompiler inlined (dsTSTRING<char>::UnsafeInitEmpty's Meyers guard) is expressed as the
// UnsafeInitEmpty() call it came from.

// boundary — comparator/swap policies passed to dsVECTOR::Sort (empty functors; behaviour inlined).
struct SRT_CMP {};
template<class T> struct dsSWAP {};

dsTSTRING<char> dsAFFIX_BUILDER::BuildString() const
{
    // Copy the affix vector so it can be sorted without mutating this builder.
    dsVECTOR<AFFIX, 8> sortedAffixes;
    sortedAffixes.__cl = this->affixes.__cl;
    sortedAffixes.pData = nullptr;
    sortedAffixes.nElem = 0;
    sortedAffixes.allocated = 0;
    sortedAffixes.Insert(0, this->affixes.pData, this->affixes.nElem);

    SRT_CMP cmp;
    dsSWAP<AFFIX> swap;
    sortedAffixes.Sort<SRT_CMP, dsSWAP<AFFIX> >(cmp, swap);

    dsTSTRING<char> result;
    result.pBuffer = nullptr;
    result.UnsafeInitEmpty(); // adopt the shared empty-string singleton

    int affixCount = sortedAffixes.nElem;
    for (int appendIndex = 0; appendIndex < affixCount; ++appendIndex) {
        const AFFIX *affix = &sortedAffixes[appendIndex];
        dsAFFIX_BUILDER::AppendAffix(result, *affix);
    }

    return result; // sret: constructed directly into the caller's return buffer
}
