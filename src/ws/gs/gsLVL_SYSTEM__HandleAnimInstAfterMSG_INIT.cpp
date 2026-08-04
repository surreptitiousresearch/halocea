#include "gsLVL_SYSTEM.h"
#include "../ds/dsAFFIX_READER.h"

// boundary — CRT allocator free, paired with the manual dsAFFIX_STRING buffer refcount
// release below (dsAFFIX_READER's own destructor is inlined at call sites, per convention).
extern "C" void dlFree(void *ptr);

// 0x8273B278 — called after an animINST's INIT message finishes dispatch: scan the
// instance's affix string for a shadow-cast lighting template tag and latch it into
// state2, then run this level system's InstLoadNotify chain for the instance.
void gsLVL_SYSTEM::HandleAnimInstAfterMSG_INIT(animINST *pInst)
{
    // ctor takes const dsTSTRING<char>&; affixes is a dsAFFIX_STRING whose .str is that string.
    dsAFFIX_READER affixReader(pInst->affixes.str);

    if ( affixReader.HaveAffix("lighting_template_lm_shadow_cast", 0) )
        pInst->state2 |= 2u;

    InstLoadNotify(pInst); // virtual call; decompiler showed a duplicate `this` arg — collapsed to the real 1-arg signature

    // Manual release of the affix reader's copy of the affix string buffer (dtor inlined by the compiler).
    dsTSTRING_BUF_HEADER<char> *affixBuffer = affixReader.affixStr.pBuffer;
    --affixBuffer->refCount;
    if ( !affixBuffer->refCount )
        dlFree(affixBuffer);
}
