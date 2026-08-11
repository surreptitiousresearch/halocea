#include "dscBRAND.h"
#include "dscDESC_FAMILY.h"
#include "dscDESC.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"
#include "../ds/MAP.h"
#include "../ds/ds_assert_boundary.h"
#include "../ps/psSECTION.h"

extern "C" void dlFree(void *ptr); // boundary — CRT/dl heap free
// boundary — formatted dsTSTRING<char> builder (returns by value/sret; variadic).
extern dsTSTRING<char> dsSPrintf(const char *fmt, ...);

// dscBRAND::Init(dscDESC_FAMILY*) @ 0x82711A70
// One-time lazy init (no-op if isInited). Recursively inits the parent brand first, then
// copy-inherits the parent's descriptor into this brand's own descriptor via the PARENT's
// fnCopyDesc (disasm 0x82711AB0 reloads fnCopyDesc from `parent`, not `this`). Links the owned
// descriptor back to this brand, pulls this brand's pending config-file section out of the
// family's psList (keyed by brand name; asserts the lookup must succeed), runs it through the
// descriptor's ProcessPS, drops the now-consumed section from psList, and marks isInited.
void dscBRAND::Init(dscDESC_FAMILY *pFamily)
{
    if (this->isInited)
        return;

    dscBRAND *parent = this->pParent;
    if (parent) {
        parent->Init(pFamily);
        // reloaded after the recursive call — matches the disasm re-read of this->pParent
        // at 0x82711AA0 rather than reusing the pre-call value
        parent = this->pParent;
        if (parent->spDesc.pointee)
            ((void (*)(dscDESC *, dscDESC *))parent->fnCopyDesc)(
                this->spDesc.pointee, parent->spDesc.pointee);
    }

    this->spDesc.pointee->pBrand = this;

    ds::impl::MAP_ITERATOR<dsTSTRING<char>, psSECTION, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>
        sectionIter = pFamily->psList.Find(&this->name);

    if (!IGNORE_STRONG_ASSERT && sectionIter.IsDone())
        // called through a null this-pointer, matching the disasm's `li r3,0` before the branch
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "!it.IsDone()",
            "D:\\Projects\\code\\common\\src.sys\\gm_sys\\dsc_family.cpp",
            396,
            empty_string);

    psSECTION sectionCopy = *sectionIter;
    dsTSTRING<char> hintErr = dsSPrintf("Parsing %s", this->name.pBuffer->str);
    this->spDesc.pointee->ProcessPS(sectionCopy, hintErr);

    // dsSPrintf's returned string is released here via direct field manipulation — the
    // disassembly (0x82711B74-0x82711B94) shows inlined refcount-- + conditional dlFree, not a
    // ~dsTSTRING call, i.e. the same body as the protected dsTSTRING::ReleaseBuffer.
    dsTSTRING_BUF_HEADER<char> *hintErrBuffer = hintErr.pBuffer;
    if (--hintErrBuffer->refCount == 0)
        dlFree(hintErrBuffer);

    pFamily->psList.Erase(sectionIter);
    this->isInited = true;
}
