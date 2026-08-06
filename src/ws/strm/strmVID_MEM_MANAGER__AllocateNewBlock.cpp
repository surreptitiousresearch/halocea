#include "../../headers/ws/strm/strmVID_MEM_MANAGER.h"

// Placement/debug global allocator the ws build links in: operator new(size, file, line).
extern void *operator new(size_t size, const char *file, unsigned int line); // size_t: MSVC operator new requires size_t first param

// dl subsystem boundary — general-purpose heap free (releases dsVECTOR backing storage).
extern "C" void dlFree(void *ptr);

// strm subsystem monotonic block-id counter (global, shared by every strmVID_MEM_MANAGER).
extern "C" unsigned int blockId;

// strmVID_MEM_MANAGER::AllocateNewBlock @ 0x8269CEA8
// Allocate a fresh strmVID_MEM_BLOCK (inlined default construction — same pattern as
// ReserveAllMemory's loop body: no separate strmVID_MEM_BLOCK constructor symbol exists in this
// build) and Init() it with the next `blockId`. On success, push it onto `blocks` and return it.
// On an Init failure, free the block's dsVECTOR backing storage, delete it, and return nullptr
// (allocation failure of the block itself is not guarded here, matching ReserveAllMemory).
strmVID_MEM_BLOCK *strmVID_MEM_MANAGER::AllocateNewBlock()
{
    strmVID_MEM_BLOCK *block = (strmVID_MEM_BLOCK *)operator new(sizeof(strmVID_MEM_BLOCK),
        "D:\\Projects\\code\\common\\src.sys\\drv\\strm_vidmem.cpp", 0x595u);

    if (block) {
        block->size = 0;
        block->offset = nullptr;
        block->pChunk = nullptr;
        block->compacted = true;
        block->id = (unsigned int)-1;
        block->lockFrames = 0;
        block->buffers.pData = nullptr;
        block->buffers.nElem = 0;
        block->buffers.allocated = 0;
        block->buffers.__cl.file = "D:\\Projects\\code\\common\\src.sys\\drv\\strm_vidmem.cpp";
        block->buffers.__cl.line = 460;
        block->asyncFreeBufList.pData = nullptr;
        block->asyncFreeBufList.nElem = 0;
        block->asyncFreeBufList.allocated = 0;
        block->asyncFreeBufList.__cl.file = "D:\\Projects\\code\\common\\src.sys\\drv\\strm_vidmem.cpp";
        block->asyncFreeBufList.__cl.line = 460;
        // freeSize is left uninitialized here (matches the compiled code — no write to that
        // offset in this constructor); Init() below sets it before any read.
        block->freeBufMaxSize = -1;
    }

    if (block->Init(blockId)) {
        ++blockId;
        blocks.PushBack(block);
        return block;
    }

    if (block) {
        dlFree(block->asyncFreeBufList.pData);
        dlFree(block->buffers.pData);
        operator delete(block);
    }
    return nullptr;
}
