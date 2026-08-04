#include "../../headers/ws/ent/entCREATE_INFO.h"

extern void dlFree(void *ptr);

// ?entGetClassTplName@@YAPADABV?$dsTSTRING@D@@@Z — resolve a class name's configured default
// template name (via entDESC::nameTplDefault, looked up through the dscDESC_FAMILY registry).
// Same-family free function, not reversed in this batch. boundary.
extern char *entGetClassTplName(const dsTSTRING<char> &nameClass);

// entCREATE_INFO::entCREATE_INFO(const char*, const char*, entCREATE_DATA*, msgDATA*) — 0x82533DB8
//
// CAVEAT: dsTSTRING<char>::operator= is compiler-INLINED at every adoption point below (no `bl`
// in the disasm) rather than called — reconstructed as the equivalent manual
// release-old/adopt-new/bump-refcount sequence, matching the established convention for this
// pattern (see propINST_CONSTRUCTOR::ChangeSkin). UnsafeInit/UnsafeInitEmpty ARE called (visible
// `bl`s), so those are ordinary method calls.
entCREATE_INFO::entCREATE_INFO(const char *_nameClass, const char *_nameTpl,
                               entCREATE_DATA *_pCD, msgDATA *_pInitData)
{
    // nameTpl starts pointed at the shared empty-string singleton; possibly replaced below.
    nameTpl.pBuffer = nullptr;
    nameTpl.UnsafeInitEmpty();

    nameClass.pBuffer = nullptr;
    nameClass.UnsafeInit(_nameClass, -1, 0);

    pCD = _pCD;
    pInitData = _pInitData;
    mpID = (unsigned int)-1;
    uid = -1;

    if (_nameTpl && _nameTpl[0] != '\0')
    {
        // Adopt the caller-supplied template name verbatim.
        dsTSTRING<char> newTpl;
        newTpl.pBuffer = nullptr;
        newTpl.UnsafeInit(_nameTpl, -1, 0);

        dsTSTRING_BUF_HEADER<char> *oldBuffer = nameTpl.pBuffer;
        if (--oldBuffer->refCount == 0)
            dlFree(oldBuffer);
        nameTpl.pBuffer = newTpl.pBuffer;
        ++nameTpl.pBuffer->refCount;

        dsTSTRING_BUF_HEADER<char> *tempBuffer = newTpl.pBuffer;
        if (--tempBuffer->refCount == 0)
            dlFree(tempBuffer);
    }
    else
    {
        // No explicit template name: fall back to the class's configured default template.
        dsTSTRING<char> nameClassCopy;
        nameClassCopy.pBuffer = nullptr;
        nameClassCopy.UnsafeInit(_nameClass, -1, 0);

        char *defaultTplName = entGetClassTplName(nameClassCopy);

        dsTSTRING<char> defaultTpl;
        defaultTpl.pBuffer = nullptr;
        defaultTpl.UnsafeInit(defaultTplName, -1, 0);

        dsTSTRING_BUF_HEADER<char> *oldBuffer = nameTpl.pBuffer;
        if (--oldBuffer->refCount == 0)
            dlFree(oldBuffer);
        nameTpl.pBuffer = defaultTpl.pBuffer;
        ++nameTpl.pBuffer->refCount;

        dsTSTRING_BUF_HEADER<char> *tempTplBuffer = defaultTpl.pBuffer;
        if (--tempTplBuffer->refCount == 0)
            dlFree(tempTplBuffer);

        dsTSTRING_BUF_HEADER<char> *tempClassBuffer = nameClassCopy.pBuffer;
        if (--tempClassBuffer->refCount == 0)
            dlFree(tempClassBuffer);
    }
}
