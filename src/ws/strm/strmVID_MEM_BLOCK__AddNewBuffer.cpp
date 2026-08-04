#include "../../headers/ws/strm/strmVID_MEM_BLOCK.h"

struct strmVID_BUF_INS_PRED {}; // boundary — empty ordering predicate; InsertSorted<> compares by
                                 // raw pointer value in the compiled code (see dsVECTOR specialization)

// Placement/debug global allocator the ws build links in: operator new(size, file, line).
extern void *operator new(size_t size, const char *file, unsigned int line); // size_t: MSVC operator new requires size_t first param

// strm subsystem monotonic buffer-id counter (global, shared by every strmVID_MEM_BLOCK).
extern unsigned int bufId;

// strmVID_MEM_BLOCK::AddNewBuffer @ 0x8269BDE8
// Allocate one strmVID_BUFFER, fill it in from the caller's parameters, assign it the next global
// buffer id, and splice it (address-sorted, duplicates ignored) into this block's `buffers` list.
strmVID_BUFFER *strmVID_MEM_BLOCK::AddNewBuffer(unsigned char state, unsigned char *offset,
                                                 unsigned long size, unsigned long align,
                                                 strmVID_BUFFER *prev, strmVID_BUFFER *next)
{
    strmVID_BUFFER *newBuffer = (strmVID_BUFFER *)operator new(sizeof(strmVID_BUFFER),
        "D:\\Projects\\code\\common\\src.sys\\drv\\strm_vidmem.cpp", 0x233u);
    if (newBuffer) {
        newBuffer->block = nullptr;
        newBuffer->id = (unsigned int)-1;
        newBuffer->state.state = 0;
        newBuffer->offset = nullptr;
        newBuffer->size = 0;
        newBuffer->align = 1;
        newBuffer->prev = nullptr;
        newBuffer->next = nullptr;
        newBuffer->waitFrames = 0;
        newBuffer->memmoveCb = nullptr;
    }
    // NOTE: faithful to the binary — the fields below are written unconditionally, even if the
    // allocation above failed (newBuffer == nullptr is not guarded here in the compiled code).
    newBuffer->id = bufId;
    newBuffer->block = this;
    newBuffer->state.state = state;
    newBuffer->offset = offset;
    newBuffer->size = size;
    newBuffer->align = align;
    newBuffer->prev = prev;
    newBuffer->next = next;

    strmVID_BUF_INS_PRED insertPredicate = {};
    buffers.InsertSorted<strmVID_BUF_INS_PRED>(newBuffer, insertPredicate,
        dsVECTOR<strmVID_BUFFER *, 8>::INS_DUP_IGNORE);
    ++bufId;
    return newBuffer;
}
