#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ent/entCREATE_INFO.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/msg/msgDATA.h"

extern "C" void dlFree(void *ptr);

// Declared in entCreate.cpp (the animINST-overload, lowest address). Same free-function family.
entENTITY *entCreate(animINST *pInst, const entCREATE_INFO &info);

// entCreate(char*, animINST*, msgDATA*) — 0x82536DC8
//
// Convenience overload: builds a default entCREATE_INFO, adopts `nameClass` and `pMsgData` into
// it, then forwards to entCreate(animINST*, const entCREATE_INFO&).
//
// CAVEAT: dsTSTRING<char>::operator= is compiler-inlined for the nameClass adoption (no `bl`) --
// reconstructed as the equivalent manual release/adopt/bump sequence (see
// entCREATE_INFO::entCREATE_INFO for the same convention). The decompiler's
// `if (&nameClassStr != &info.nameClass)` self-assignment guard is always true here (the two
// are never the same object) and is omitted.
entENTITY *entCreate(char *nameClass, animINST *pInst, msgDATA *pMsgData)
{
    entCREATE_INFO info;

    dsTSTRING<char> nameClassStr;
    nameClassStr.pBuffer = nullptr;
    nameClassStr.UnsafeInit(nameClass, -1, 0);

    dsTSTRING_BUF_HEADER<char> *oldNameClassBuffer = info.nameClass.pBuffer;
    if (--oldNameClassBuffer->refCount == 0)
        dlFree(oldNameClassBuffer);
    info.nameClass.pBuffer = nameClassStr.pBuffer;
    ++info.nameClass.pBuffer->refCount;

    dsTSTRING_BUF_HEADER<char> *tempBuffer = nameClassStr.pBuffer;
    if (--tempBuffer->refCount == 0)
        dlFree(tempBuffer);

    info.pInitData = pMsgData;

    return entCreate(pInst, info);
}
