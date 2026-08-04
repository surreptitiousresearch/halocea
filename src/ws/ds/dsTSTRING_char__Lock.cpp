#include "dsTSTRING.h"
#include <string.h>

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// boundary — Saber processor-affinity debug guard used by the string Lock/Unlock pair.
struct apPROCESSOR_LOCK {
    static void CheckCurrentProcessor(apPROCESSOR_LOCK *lock, const char *file, int line);
};

// dsTSTRING<char>::Lock @ 0x823C3AC0
// Open an edit window over a private, writable buffer of at least `strLen` characters and return a
// pointer to its data. If the buffer is uniquely owned, grow it in place when too small; otherwise
// copy-on-write into a fresh buffer and drop the shared reference. The live length is then clamped
// to `strLen` and the buffer NUL-terminated at `strLen`. Pair with Unlock.
template<>
char *dsTSTRING<char>::Lock(int strLen)
{
    apPROCESSOR_LOCK::CheckCurrentProcessor(
        &dsTSTRING<char>::processorLock,
        "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp",
        815);

    if (this->pBuffer->refCount == 1) {
        if (this->pBuffer->maxStrLen < strLen)
            this->pBuffer = this->ReallocBuffer(this->pBuffer, strLen, this->pBuffer->strLen);
    } else {
        dsTSTRING_BUF_HEADER<char> *oldBuffer = this->pBuffer;
        int oldStrLen = this->pBuffer->strLen;
        apPROCESSOR_LOCK::CheckCurrentProcessor(
            &dsTSTRING<char>::processorLock,
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp",
            928);
        this->pBuffer = this->UnsafeAllocBuffer(strLen, oldStrLen);
        memcpy(this->pBuffer->str, oldBuffer->str, oldBuffer->strLen);
        apPROCESSOR_LOCK::CheckCurrentProcessor(
            &dsTSTRING<char>::processorLock,
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp",
            1014);
        if (!--oldBuffer->refCount)
            dlFree(oldBuffer);
    }

    if (strLen < this->pBuffer->strLen)
        this->pBuffer->strLen = strLen;
    this->pBuffer->str[strLen] = 0;
    return this->pBuffer->str;
}
