#include "dsTSTRING.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT
extern "C" void *memcpy(void *dst, const void *src, unsigned int n);  // boundary — CRT
extern "C" void dlFree(void *ptr);                                    // boundary — dlmalloc allocator free

// dsTSTRING<char>::Insert @ 0x823B0F78  (protected grow-and-shift; overload #3 by ascending address)
// NOTE ordering caveat: this overload has the lowest address of the three Insert overloads, but the
// two public overloads were already committed as the bare / __2 names; kept as __3 to avoid churn.
// Open a `len`-character gap at `pos`. Uniquely-owned buffers grow (ReallocBuffer) or reuse spare
// capacity in place, then shift the tail up past the gap; a shared buffer is copied-on-write into a
// fresh larger buffer with the gap left open, dropping the old reference.
template<>
void dsTSTRING<char>::Insert(int pos, int len)
{
    if (!len)
        return;

    dsTSTRING_BUF_HEADER<char> *buffer = this->pBuffer;
    int strLen = this->pBuffer->strLen;
    if (this->pBuffer->refCount == 1) {
        if (buffer->maxStrLen < strLen + len) {
            dsTSTRING_BUF_HEADER<char> *grown = this->ReallocBuffer(this->pBuffer, strLen + len, strLen + len);
            this->pBuffer = grown;
            memmove(&grown->str[pos + len], &grown->str[pos], grown->strLen - pos - len);
        } else {
            buffer->strLen = strLen + len;
            this->pBuffer->str[this->pBuffer->strLen] = 0;
            memmove(&this->pBuffer->str[pos + len], &this->pBuffer->str[pos],
                    this->pBuffer->strLen - pos - len);
        }
    } else {
        dsTSTRING_BUF_HEADER<char> *fresh = this->AllocBuffer(strLen + len, strLen + len);
        this->pBuffer = fresh;
        memcpy(fresh->str, buffer->str, pos);
        memcpy(&this->pBuffer->str[pos + len], &buffer->str[pos], buffer->strLen - pos);
        if (buffer->refCount-- == 1)
            dlFree(buffer);
    }
}
