#include <stdint.h>
#include "MSG_PROTOCOL.h"
#include "../ds/ds_assert_boundary.h"

// os primitives -- boundary (inline extern, matching sibling ws/* convention).
extern "C" uint64_t osGetPerfCounter(void);     // boundary
extern "C" uint64_t osGetPerfFrequancy(void);   // boundary
extern "C" int strcmp(const char *a, const char *b);    // boundary — CRT

namespace con {

// con::MSG_PROTOCOL::SendMsg(fioFILE&) @ 0x82AC8408
// Send a previously-started message stream (from StartMsg) to pComm as an id==1
// (fire-and-forget) message, then release the pooled slot back to "free".
//
// CAVEAT: two call sites in the decompile pass an extra bogus 2nd argument to
// fioFILE_MEM::GetStreamName()/GetBuf() (both real 0-arg virtuals per fioFILE_MEM_vtbl) --
// disasm (82ac8494-82ac84cc, 82ac8520-82ac8534) confirms only `this` is ever loaded before
// the vtable call; the extra argument is a decompiler ctree artifact. Reconstructed here as
// the plain 0-arg calls. Likewise the osGetPerfCounter()/osGetPerfFrequancy() calls whose
// results are never stored anywhere observable are dead reads (matches the pattern already
// documented in the sibling MSG_PROCESSOR_BASE::SendMsg CAVEAT) -- preserved as no-op calls
// only on the code path disasm shows them actually executing (skipped entirely when the
// stream is found in the pool).
void MSG_PROTOCOL::SendMsg(fioFILE &f)
{
    // Mangled signature takes fioFILE&, but msgFile stores fioFILE_MEM* and every real caller
    // passes a fioFILE_MEM instance; fioFILE is the base subobject at offset 0 of fioFILE_MEM.
    fioFILE_MEM *target = (fioFILE_MEM *)&f;

    lock.Lock(nullptr, 0);

    int index = 0;
    bool found = false;
    if (msgFile.nElem > 0)
    {
        while (msgFile[index] != target)
        {
            if (++index >= msgFile.nElem)
                break;
        }
        found = (index < msgFile.nElem);
    }

    if (found)
    {
        msgFile[index]->Term();
        if (pComm)
        {
            fioFILE_MEM *slot = msgFile[index];
            pComm->SendMsg(slot->GetBuf(), slot->streamSize, 1);
        }
    }
    else if (pComm)
    {
        if (!IGNORE_STRONG_ASSERT)
        {
            const char *streamName = f.GetStreamName();
            if (!streamName || strcmp(streamName, "memory buffer") != 0)
            {
                STRONG_ASSERT_DUMMY().Crash(
                    "f.GetStreamName() && strcmp(f.GetStreamName(), \"memory buffer\") == 0",
                    "D:\\Projects\\code\\common\\src.sys\\console\\conComm.cpp",
                    274,
                    dsStrongAssertMessage);
            }
        }
        pComm->SendMsg(target->GetBuf(), f.streamSize, 1);
        osGetPerfCounter();
        osGetPerfFrequancy();
        osGetPerfFrequancy();
    }

    lock.Unlock(nullptr, 0);
}

} // namespace con
