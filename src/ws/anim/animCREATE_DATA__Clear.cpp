/* ?Clear@animCREATE_DATA@@QAAXXZ @0x8252D3A8 */
#include "animCREATE_DATA.h"
#include "../ds/dsTSTRING.h"
#include "anim_boundary.h"
#include <string.h>

// operator delete (global) — boundary.
void operator delete(void *p);

// Process-wide shared empty-string singleton adopted when a dsTSTRING/affix is reset (lazily
// allocated by dsTSTRING<char>::UnsafeInitEmpty). boundary.
extern dsTSTRING<char> dsEmptyString;

// ?Clear@animCREATE_DATA@@QAAXXZ  (0x8252D3A8) — reset the creation-data block to defaults.
// CAVEAT: the affixes reset below is the compiler-inlined dsAFFIX_STRING assignment from the shared
// empty string; the buffer/refCount juggling (temporary construct + two temporary destructions) is
// reproduced faithfully. Net effect: this->affixes adopts the shared empty-string buffer.
void animCREATE_DATA::Clear()
{
    this->isIgnorePS_Scale = 0;
    this->scaleZ = 1.0f;
    this->state = 0;
    this->scaleY = 1.0f;
    this->animSeqNmb = -1;
    this->scaleX = 1.0f;
    this->animSeqIsCycled = 0;
    this->frameCur = 0.0f;
    this->matrInst.Identity();
    memset(this->name, 0, sizeof(this->name));

    psSECTION defaultSection;
    this->ps = defaultSection;
    // defaultSection destructed here (end of its scope in the original)

    // --- reset affixes to the shared empty string ---
    dsTSTRING_BUF_HEADER<char> *pEmptyBuffer = dsEmptyString.pBuffer;
    ++pEmptyBuffer->refCount;                 // temporary dsTSTRING shares the empty buffer

    // temporary dsAFFIX_STRING construction from the temporary string shares the buffer again
    dsTSTRING_BUF_HEADER<char> *pAdoptedBuffer = pEmptyBuffer;
    ++pAdoptedBuffer->refCount;

    if (pAdoptedBuffer != (dsTSTRING_BUF_HEADER<char> *)this->affixes.str.pBuffer)
    {
        dsTSTRING_BUF_HEADER<char> *pOldBuffer = this->affixes.str.pBuffer;
        if (--pOldBuffer->refCount == 0)
            dlFree(pOldBuffer);
        this->affixes.str.pBuffer = pAdoptedBuffer;
        ++pAdoptedBuffer->refCount;
    }
    // temporary dsAFFIX_STRING destruction
    if (pAdoptedBuffer->refCount-- == 1)
        dlFree(pAdoptedBuffer);
    // temporary dsTSTRING destruction
    if (pEmptyBuffer->refCount-- == 1)
        dlFree(pEmptyBuffer);

    // --- release any shader-replacement data ---
    rendSR_DATA *srData = this->srData;
    if (srData)
    {
        dlFree(srData->lampsData.pData);
        operator delete(srData);
    }
    this->srData = nullptr;
}
