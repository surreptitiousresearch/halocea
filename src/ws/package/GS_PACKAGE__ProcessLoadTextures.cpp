// package::GS_PACKAGE::ProcessLoadTextures @ 0x82778590
// If the "textures pending" state bit (0x100) is set, load every texture named by the current
// package: build the on-pak path "<curRoot>\textures\<name>.pct", read it into a memory stream,
// register the texture with txmManager (Add), load its pixels from the stream if not already
// resident, and record the ref-counted handle in the package's textures vector. Finally close the
// pak (through package::pkg_mng) using a string that shares the curRoot buffer.

#include "../../headers/ws/package/GS_PACKAGE.h"
#include "../../headers/ws/fio/fioFILE_MEM.h"
#include "../../headers/ws/txm/txmMANAGER.h"
#include "../../headers/ws/txm/txmTEXTURE.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/ws/fio/FIO_OPEN.h"

// boundary externs (CRT/OS + allocator helpers, external to this batch)
extern "C" long osLockedIncrement(volatile int *pValue);
extern "C" void dlFree(void *ptr);

void package::GS_PACKAGE::ProcessLoadTextures()
{
    // Condition in the binary is (_cntlzw(state.state & 0x100) & 0x20) == 0, i.e. bit 0x100 set.
    if (!(state.state & 0x100))
        return;

    state.state &= ~0x100u; // consume the "textures pending" request

    int curIdx = this->curIdx;

    for (int i = 0; ; ++i)
    {
        GS_PACKAGE_INFO &info = oldList[curIdx];
        int nElem = info.textureNames.nElem;
        if (i >= nElem)
            break;

        // Build the pak path in `path`, sharing the curRoot string buffer (manual copy-ctor: adopt
        // the buffer pointer and bump its refCount), matching the inlined dsTSTRING copy in the
        // binary.
        dsTSTRING<char> path;
        path.pBuffer = curRoot.pBuffer;
        ++path.pBuffer->refCount;

        path.Insert(path.pBuffer->strLen, "\\textures\\", -1);
        const dsTSTRING<char> &texName = oldList[curIdx].textureNames[i];
        path.Insert(path.pBuffer->strLen, texName);
        path.Insert(path.pBuffer->strLen, ".pct", -1);

        fioFILE_MEM memFile;
        dsFLAGS<FIO_OPEN, int> openFlags;
        openFlags.val = 0x29; // FIO_OPEN flag set observed at the call site
        memFile.InitMemory(nullptr, 0, openFlags);

        if (memFile.ReadFile(path.pBuffer->str))
        {
            const char *name = oldList[curIdx].textureNames[i].pBuffer->str;
            txmTEXTURE_PTR texHandle;
            texHandle.ptr = txmManager->Add(name, 0x2000, 0);
            if (texHandle.ptr)
                osLockedIncrement(&texHandle.ptr->refCount);

            // Load pixels only if not already resident (state bit 0x200000 clear).
            if (!(texHandle.ptr->state.state & 0x200000))
                txmManager->LoadFromPak(texHandle.ptr, &memFile, 0);

            oldList[curIdx].textures.PushBack(texHandle);

            if (texHandle.ptr && txmManager)
                texHandle.ptr->Release();
        }

        memFile.~fioFILE_MEM();

        // Drop the shared curRoot buffer reference; free the header when it hits zero.
        dsTSTRING_BUF_HEADER<char> *pathBuf = path.pBuffer;
        --pathBuf->refCount;
        if (!pathBuf->refCount)
            dlFree(pathBuf);
    }

    // Close the pak. The argument is a temporary dsTSTRING that shares the curRoot buffer.
    dsTSTRING<char> closeName;
    closeName.pBuffer = curRoot.pBuffer;
    ++closeName.pBuffer->refCount;
    package::pkg_mng->Close(closeName, 1);
}
