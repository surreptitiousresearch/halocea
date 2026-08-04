#include "animINST.h"
#include "../ds/dsAFFIX_STRING.h"
#include "../ds/dsTSTRING.h"
#include "anim_boundary.h"

// ?SetAffixes@animINST@@QAAXABVdsAFFIX_STRING@@@Z  (0x825E38A0) — replace the instance's affix
// string: drop a reference on the current backing buffer (freeing it at zero) and share `str`'s
// buffer instead. No-op when assigning from the instance's own affix string.
void animINST::SetAffixes(const dsAFFIX_STRING &str)
{
    dsAFFIX_STRING *pAffixes = &this->affixes;
    if (&str != pAffixes)
    {
        dsTSTRING_BUF_HEADER<char> *pOldBuffer = pAffixes->str.pBuffer;
        int oldRefCount = pAffixes->str.pBuffer->refCount - 1;
        pAffixes->str.pBuffer->refCount = oldRefCount;
        if (!oldRefCount)
            dlFree(pOldBuffer);
        dsTSTRING_BUF_HEADER<char> *pNewBuffer = str.str.pBuffer;
        pAffixes->str.pBuffer = str.str.pBuffer;
        ++pNewBuffer->refCount;
    }
}
