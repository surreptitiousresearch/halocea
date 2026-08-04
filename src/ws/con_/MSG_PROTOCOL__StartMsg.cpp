#include "MSG_PROTOCOL.h"
#include <new> // placement new (new (newFile) fioFILE_MEM())

// Placement/debug global allocator the ws build links in: operator new(size, file, line).
extern void *operator new(size_t size, const char *file, unsigned int line);

namespace con {

// con::MSG_PROTOCOL::StartMsg(const char*, const char*, const char*) @ 0x82AC8D88
// Claim a free slot from the shared message-buffer pool (growing the pool with a freshly
// allocated fioFILE_MEM if every existing slot is still in use), reset it to a 1KB write
// buffer, write the page/ctrl/msg header strings, and return it for the caller to append a
// payload before handing it to SendMsg/SendImmMsg.
fioFILE &MSG_PROTOCOL::StartMsg(const char *page, const char *ctrl, const char *msg)
{
    lock.Lock(nullptr, 0);

    // A pooled slot is "free" when bit 1 (0x2, FIO_OPEN_W) of its state is clear.
    int index = 0;
    int count = msgFile.nElem;
    for (; index < msgFile.nElem; ++index)
    {
        count = msgFile.nElem;
        if (((unsigned int)msgFile[index]->state.val >> 1 & 1) == 0)
            break;
    }

    if (index == count)
    {
        // DEVIATION: the decompile reads "v21 = (int)v12" here, but v12 is never assigned in
        // the ctree -- disasm (82ac8e1c..82ac8e24) shows the constructor's return value (r3,
        // the newly allocated block itself) stored directly; there is no separate v12. This is
        // the ordinary null-checked placement-new pattern for this codebase's debug allocator.
        fioFILE_MEM *newFile = (fioFILE_MEM *)operator new(0x7Cu,
            "D:\\Projects\\code\\common\\src.sys\\console\\conComm.cpp", 0xE0u);
        if (newFile)
            new (newFile) fioFILE_MEM();
        msgFile.PushBack(newFile);
    }

    if ((int)msgFile[index]->allocSize > 1024)
        msgFile[index]->InitAllocate(0, dsFLAGS<FIO_OPEN, int>{FIO_OPEN_W});

    msgFile[index]->InitAllocate(1024, dsFLAGS<FIO_OPEN, int>{FIO_OPEN_W});
    msgFile[index]->state.val |= 0x20u; // FIO_OPEN_BYTE_REORDER
    msgFile[index]->Write(page);
    msgFile[index]->Write(ctrl);
    msgFile[index]->Write(msg);

    fioFILE_MEM *claimedSlot = msgFile[index];
    lock.Unlock(nullptr, 0);
    return *claimedSlot;
}

} // namespace con
