#include "../../headers/ws/strm/strmVID_MEM_MANAGER.h"

// Placement/debug global allocator the ws build links in: operator new(size, file, line).
extern void *operator new(size_t size, const char *file, unsigned int line); // size_t: MSVC operator new requires size_t first param

// dl subsystem boundary — general-purpose heap free (releases dsVECTOR backing storage).
extern void dlFree(void *ptr);

// strm subsystem monotonic block-id counter (global, shared by every strmVID_MEM_MANAGER).
extern unsigned int blockId;

// strmVID_MEM_MANAGER::ReserveAllMemory @ 0x8269D5A0
// Repeatedly allocate a fresh strmVID_MEM_BLOCK and Init() it (each Init call reserves its own
// physical-memory range) until Init fails, pushing every successfully-Init'd block onto `blocks`.
// The final (failed) block's dsVECTOR backing stores are freed and the block itself deleted.
void strmVID_MEM_MANAGER::ReserveAllMemory()
{
    strmVID_MEM_BLOCK *block;
    for (;;) {
        // Inlined strmVID_MEM_BLOCK default construction — no separate constructor symbol exists
        // in this build; the compiler folded `new(file,line) strmVID_MEM_BLOCK()` into raw field
        // writes here. sizeof(strmVID_MEM_BLOCK) == 0x48 confirms this is that class.
        block = (strmVID_MEM_BLOCK *)operator new(sizeof(strmVID_MEM_BLOCK),
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
        // NOTE: faithful to the binary — Init() is invoked unconditionally, even when `block` is
        // null (allocation failure is not guarded here in the compiled code).
        if (!block->Init(blockId))
            break;
        ++blockId;
        blocks.PushBack(block);
        if (!block)
            return;
    }

    if (block) {
        dlFree(block->asyncFreeBufList.pData);
        dlFree(block->buffers.pData);
        operator delete(block);
    }
}
