#include "dsTSTRING.h"
#include <string.h> // memcpy

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsTSTRING<char>::Reserve @ 0x82520E58
// Ensure the backing buffer can hold at least `strLen` characters.
template<>
void dsTSTRING<char>::Reserve(int strLen)
{
    dsTSTRING_BUF_HEADER<char> *oldBuffer = this->pBuffer;
    if (oldBuffer->maxStrLen < strLen) {
        if (oldBuffer->refCount == 1) {
            // Uniquely owned: grow the existing buffer in place.
            this->pBuffer = this->ReallocBuffer(oldBuffer, strLen, oldBuffer->strLen);
        } else {
            // Shared: copy-on-write into a fresh buffer and drop a reference on the old one.
            dsTSTRING_BUF_HEADER<char> *newBuffer = this->AllocBuffer(strLen, oldBuffer->strLen);
            this->pBuffer = newBuffer;
            memcpy(newBuffer->str, oldBuffer->str, oldBuffer->strLen);
            if (oldBuffer->refCount-- == 1)
                dlFree(oldBuffer);
        }
    }
}
