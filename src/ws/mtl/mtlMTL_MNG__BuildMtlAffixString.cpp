#include "mtlMTL_MNG.h"
#include "../obj/objOBJ.h"
#include <cstring>

// mtlMTL_MNG::BuildMtlAffixString(objOBJ *, const dsTSTRING<char> &) — 0x82A86B30
// Returns instAffixes concatenated with pObj's own affix string (objOBJ::affixes @0x34).
// ABI: r3 = sret result buffer, r4 = this (loaded but never read by the body, verified against
// disasm — r4 is clobbered before any use), r5 = pObj, r6 = instAffixes.
dsTSTRING<char> mtlMTL_MNG::BuildMtlAffixString(objOBJ *pObj, const dsTSTRING<char> &instAffixes)
{
    dsTSTRING<char> result;

    // Copy-construct: share instAffixes' buffer and bump its refcount (matches disasm: stw pBuffer,
    // then ++pBuffer->refCount, rather than calling a full copy-ctor).
    result.pBuffer = instAffixes.pBuffer;
    ++result.pBuffer->refCount;

    const dsTSTRING_BUF_HEADER<char> *objAffixBuffer = pObj->affixes.str.pBuffer;
    int objAffixLen = objAffixBuffer->strLen;
    int insertPos = result.pBuffer->strLen;

    // Public Insert(pos, s, len): append pObj's affix chars at the end of result. (The protected
    // gap-open Insert(pos,len)+memcpy the decompiler showed is inaccessible outside dsTSTRING; this
    // public overload is equivalent.)
    result.Insert(insertPos, objAffixBuffer->str, objAffixLen);

    return result;
}
