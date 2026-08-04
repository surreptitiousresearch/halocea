#include "MSG_PROTOCOL.h"
#include "SINGLE_ENTRY_CHECKER.h"

// os-layer atomic primitive -- boundary (matches sibling SINGLE_ENTRY_CHECKER ctor usage).
extern "C" int osLockedSub(int *slot, int delta); // boundary

namespace con {

// con::MSG_PROTOCOL::SendImmMsg(fioFILE&) @ 0x82AC8698
// Send a previously-started message stream as an id==2 request and block (inside
// MSG_PROCESSOR_BASE::SendImmMsg) for the matching response, returning a pooled memory
// stream over the response bytes -- or null on failure/disconnect.
//
// CAVEAT: `immResp`/`immFile` are guarded, lazily-constructed function-local statics with
// atexit-registered destructors in the decompile (guard bits `_S3_41` bit0/bit1); modeled
// here as ordinary C++ function-local statics, which the compiler lowers to the identical
// guard+atexit pattern -- semantically equivalent, much clearer than reproducing the raw
// guard bytes.
fioFILE *MSG_PROTOCOL::SendImmMsg(fioFILE &f)
{
    if (!pComm)
        return nullptr;

    static CON_MSG immResp;
    static fioFILE_MEM immFile;

    SINGLE_ENTRY_CHECKER checker(nEntry_0);

    // Mangled signature takes fioFILE&, but msgFile stores fioFILE_MEM* and every real caller
    // passes a fioFILE_MEM instance; fioFILE is the base subobject at offset 0 of fioFILE_MEM.
    fioFILE_MEM *target = (fioFILE_MEM *)&f;

    lock.Lock(nullptr, 0);
    fioFILE_MEM *found = nullptr;
    int index = 0;
    if (msgFile.nElem > 0)
    {
        while (msgFile[index] != target)
        {
            if (++index >= msgFile.nElem)
                goto notFound;
        }
        found = msgFile[index];
    }
notFound:
    lock.Unlock(nullptr, 0);

    if (!found || !pComm ||
        !pComm->SendImmMsg(found->GetBuf(), found->streamSize, immResp, nullptr))
    {
        osLockedSub(checker.nEntry, 1);
        return nullptr;
    }

    immFile.Term();
    immFile.InitMemory(immResp.data.pData, immResp.data.nElem, dsFLAGS<FIO_OPEN, int>{FIO_OPEN_R});
    immFile.state.val |= 0x20u; // FIO_OPEN_BYTE_REORDER

    lock.Lock(nullptr, 0);
    index = 0;
    if (msgFile.nElem > 0)
    {
        while (msgFile[index] != target)
        {
            if (++index >= msgFile.nElem)
                goto unlock;
        }
        msgFile[index]->Term();
    }
unlock:
    lock.Unlock(nullptr, 0);

    osLockedSub(checker.nEntry, 1);
    return &immFile;
}

} // namespace con
