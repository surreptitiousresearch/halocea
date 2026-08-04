#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr);                 // boundary — dlmalloc allocator free
extern "C" void *memcpy(void *dst, const void *src, unsigned int n); // boundary — CRT

// dsTSTRING<char>::At @ 0x82517E08
// Writable pointer to character `idx`. If the backing buffer is shared (refCount != 1),
// copy-on-write into a private buffer first, then drop the old reference.
template<>
char *dsTSTRING<char>::At(int idx)
{
    dsTSTRING_BUF_HEADER<char> *oldBuffer = this->pBuffer;
    if (oldBuffer->refCount != 1) {
        dsTSTRING_BUF_HEADER<char> *privateBuffer =
            this->AllocBuffer(oldBuffer->strLen, oldBuffer->strLen);
        this->pBuffer = privateBuffer;
        memcpy(privateBuffer->str, oldBuffer->str, oldBuffer->strLen);
        if (oldBuffer->refCount-- == 1)
            dlFree(oldBuffer);
    }
    return &this->pBuffer->str[idx];
}
