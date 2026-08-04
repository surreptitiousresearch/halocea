#include "psITERATOR.h"
#include "psSECTION_KEY_REF.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"
// 0x823D4E80 (??1psITERATOR@@QAA@XZ) — destroy the collected record vector, then release the
// name filter's string buffer (freeing it at zero refcount).

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

psITERATOR::~psITERATOR()
{
    this->records.~dsVECTOR();

    dsTSTRING_BUF_HEADER<char> *pBuffer = this->name.pBuffer;
    if (--pBuffer->refCount == 0)
        dlFree(pBuffer);
}
