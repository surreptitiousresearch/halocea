/* CAVEAT: sret / this-pointer swap. Mangle ?GetDbgString@dsDATA@@QBA?AV?$dsTSTRING@D@@XZ is a
   const method returning dsTSTRING<char> BY VALUE. Under the PPC sret ABI r3 is the hidden
   return-buffer pointer and r4 is the real `this`, so the decompiler mislabels them (its `this`
   is the return buffer, its `result` is the true dsDATA). Reconstructed with the true signature.
   The stored type's own debug-string producer is virtual slot 0x68 (dsDATA_TYPE::GetDbgString);
   the "copy" of the produced string into the return value is an inlined refcount-sharing
   (retStr.pBuffer = src.pBuffer; ++refCount) followed by release of the temporary. */
#include "dsDATA.h"
#include "dsDATA_TYPE.h"
#include "dsTSTRING.h"
#include "dsTSTRING_BUF_HEADER.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsDATA::GetDbgString @ 0x826219F8
// Produce a human-readable debug string for the stored value. A typed value delegates to its type
// descriptor's GetDbgString; an empty value renders as the literal "null". The result string shares
// the produced buffer (refcount bump), and the transient producer string is released.
dsTSTRING<char> dsDATA::GetDbgString() const
{
    dsTSTRING<char> retStr;                 // hidden sret return buffer

    const dsDATA_TYPE *storedType = this->type;
    unsigned int cleanupFlags;              // bit0: release typeDbg, bit1: release nullStr

    dsTSTRING<char> typeDbg;                // producer string from the type descriptor
    dsTSTRING<char> nullStr;                // "null" literal for the empty case
    dsTSTRING<char> *source;

    if (storedType) {
        cleanupFlags = 1;
        typeDbg = storedType->GetDbgString();
        source = &typeDbg;
    } else {
        nullStr.pBuffer = nullptr;
        cleanupFlags = 2;
        nullStr.UnsafeInit("null", -1, 0);
        source = &nullStr;
    }

    dsTSTRING_BUF_HEADER<char> *sharedBuf = source->pBuffer;
    int refCount = sharedBuf->refCount;
    retStr.pBuffer = sharedBuf;             // share the produced buffer
    sharedBuf->refCount = refCount + 1;

    if (cleanupFlags & 2) {
        dsTSTRING_BUF_HEADER<char> *buf = nullStr.pBuffer;
        cleanupFlags &= ~2u;
        if (nullStr.pBuffer->refCount-- == 1)
            dlFree(buf);
    }
    if (cleanupFlags & 1) {
        dsTSTRING_BUF_HEADER<char> *buf = typeDbg.pBuffer;
        if (--buf->refCount == 0)
            dlFree(buf);
    }
    return retStr;
}
