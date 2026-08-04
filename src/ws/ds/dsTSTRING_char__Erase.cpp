#include "dsTSTRING.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT
extern "C" void *memcpy(void *dst, const void *src, unsigned int n);  // boundary — CRT
extern "C" void dlFree(void *ptr);                                    // boundary — dlmalloc allocator free

// dsTSTRING<char>::Erase @ 0x82517F90
// Remove `len` characters starting at `pos`. len==-1 (or a len past the end) clamps to "to end of
// string". Erasing the whole string delegates to Clear(). When the buffer is uniquely owned the tail
// is shifted down in place and the length trimmed; otherwise a fresh smaller buffer is allocated and
// the surviving head/tail copied into it (copy-on-write), dropping the old reference. Returns *this.
template<>
dsTSTRING<char> &dsTSTRING<char>::Erase(int pos, int len)
{
    dsTSTRING_BUF_HEADER<char> *buffer = this->pBuffer;
    int strLen = buffer->strLen;
    int available = strLen - pos;
    int eraseLen = len;
    if (len == -1 || len > available)
        eraseLen = available;

    if (!pos && eraseLen == strLen)
        return this->Clear();

    if (buffer->refCount == 1) {
        memmove(&buffer->str[pos], &buffer->str[pos + eraseLen], available - eraseLen);
        this->pBuffer->strLen -= eraseLen;
        this->pBuffer->str[this->pBuffer->strLen] = 0;
        return *this;
    }

    // Shared buffer: copy-on-write into a fresh buffer sized for the survivors.
    dsTSTRING_BUF_HEADER<char> *fresh = this->AllocBuffer(buffer->strLen - eraseLen, buffer->strLen - eraseLen);
    this->pBuffer = fresh;
    memcpy(fresh->str, buffer->str, pos);
    memcpy(&this->pBuffer->str[pos], &buffer->str[pos + eraseLen], buffer->strLen - pos - eraseLen);
    if (buffer->refCount-- == 1)
        dlFree(buffer);
    return *this;
}
