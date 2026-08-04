#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsTSTRING<char>::operator= @ 0x823B01D0
// Copy-assign: unless self-assigning, drop this string's current buffer reference (freeing at
// zero), then adopt the source's shared buffer and bump its reference count. Returns *this.
template<>
dsTSTRING<char> &dsTSTRING<char>::operator=(const dsTSTRING<char> &other)
{
    if (&other != this) {
        dsTSTRING_BUF_HEADER<char> *oldBuffer = this->pBuffer;
        if (oldBuffer->refCount-- == 1)
            dlFree(oldBuffer);
        dsTSTRING_BUF_HEADER<char> *newBuffer = other.pBuffer;
        this->pBuffer = other.pBuffer;
        ++newBuffer->refCount;
    }
    return *this;
}
