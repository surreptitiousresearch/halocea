#include "dsTSTRING.h"

// boundary — Saber processor-affinity debug guard used by the string Lock/Unlock pair.
struct apPROCESSOR_LOCK {
    static void CheckCurrentProcessor(apPROCESSOR_LOCK *lock, const char *file, int line);
};

// dsTSTRING<char>::Unlock @ 0x823C1A80
// Close a Lock() edit window: recompute the buffer's live length by scanning for the first NUL
// within the capacity, or clamp to the full capacity (and re-terminate) when no NUL is found.
template<>
void dsTSTRING<char>::Unlock()
{
    apPROCESSOR_LOCK::CheckCurrentProcessor(
        &dsTSTRING<char>::processorLock,
        "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp",
        864);

    for (int i = 0; i <= this->pBuffer->maxStrLen; ++i) {
        if (!this->pBuffer->str[i]) {
            this->pBuffer->strLen = i;
            return;
        }
    }
    this->pBuffer->strLen = this->pBuffer->maxStrLen;
    this->pBuffer->str[this->pBuffer->strLen] = 0;
}
