#include "MSG_PROTOCOL.h"

namespace con {

// Definition (single storage point for this reconstruction, which splits the original
// single-TU conComm.cpp across sibling .cpp files) -- see the extern declarations and
// commentary in MSG_PROTOCOL.h.
dsVECTOR<fioFILE_MEM *, 8> msgFile;
int nEntry_0;

// con::MSG_PROTOCOL::CancelMsg(fioFILE&) @ 0x82AC85F8
// Find `f` in the shared message-buffer pool and terminate it (releases the pooled slot
// back to "free" so a later StartMsg can reuse it).
void MSG_PROTOCOL::CancelMsg(fioFILE &f)
{
    // CAVEAT: the mangled signature takes fioFILE&, but msgFile stores fioFILE_MEM* and every
    // real caller passes a fioFILE_MEM instance obtained from StartMsg/SendImmMsg; comparing
    // by address is valid because fioFILE is the base subobject at offset 0 of fioFILE_MEM.
    fioFILE_MEM *target = (fioFILE_MEM *)&f;

    lock.Lock(nullptr, 0);
    int index = 0;
    if (msgFile.nElem > 0)
    {
        while (msgFile[index] != target)
        {
            if (++index >= msgFile.nElem)
                goto done;
        }
        msgFile[index]->Term();
    }
done:
    lock.Unlock(nullptr, 0);
}

} // namespace con
