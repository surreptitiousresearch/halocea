#include "dsTSTRING.h"
#include <string.h>

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsTSTRING<char>::Insert @ 0x823B4D80  (const char* overload; overload #4 by discovery order)
// Insert up to `len` characters of the NUL-terminated `str` at position `pos` (len==-1 = whole
// string; the run stops at the first NUL in `str`). Opens a `insertLen`-char gap at `pos` — growing
// the buffer in place when uniquely owned, else copy-on-write into a fresh buffer — then copies the
// characters in. Returns *this.
template<>
dsTSTRING<char> &dsTSTRING<char>::Insert(int pos, const char *str, int len)
{
    if (str) {
        if (len == -1)
            len = 0x7FFFFFFF;
    } else {
        len = 0;
    }

    // Number of characters actually taken from `str`: min(strlen(str), len).
    int insertLen = 0;
    if (len > 0) {
        do {
            if (!str[insertLen])
                break;
            ++insertLen;
        } while (insertLen < len);

        if (insertLen) {
            dsTSTRING_BUF_HEADER<char> *oldBuffer = this->pBuffer;
            int oldStrLen = this->pBuffer->strLen;
            if (this->pBuffer->refCount == 1) {
                if (oldBuffer->maxStrLen < insertLen + oldStrLen) {
                    dsTSTRING_BUF_HEADER<char> *grown =
                        this->ReallocBuffer(this->pBuffer, insertLen + oldStrLen, insertLen + oldStrLen);
                    this->pBuffer = grown;
                    memmove(&grown->str[pos + insertLen], &grown->str[pos], grown->strLen - insertLen - pos);
                } else {
                    oldBuffer->strLen = insertLen + oldStrLen;
                    this->pBuffer->str[this->pBuffer->strLen] = 0;
                    memmove(&this->pBuffer->str[pos + insertLen], &this->pBuffer->str[pos],
                            this->pBuffer->strLen - insertLen - pos);
                }
            } else {
                // shared buffer: copy-on-write into a private buffer with the gap already opened
                dsTSTRING_BUF_HEADER<char> *fresh =
                    this->AllocBuffer(oldStrLen + insertLen, oldStrLen + insertLen);
                this->pBuffer = fresh;
                memcpy(fresh->str, oldBuffer->str, pos);
                memcpy(&this->pBuffer->str[insertLen + pos], &oldBuffer->str[pos], oldBuffer->strLen - pos);
                if (oldBuffer->refCount-- == 1)
                    dlFree(oldBuffer);
            }
        }
    }

    memcpy(&this->pBuffer->str[pos], str, insertLen);
    return *this;
}
