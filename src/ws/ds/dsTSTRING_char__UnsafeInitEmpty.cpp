#include "dsTSTRING.h"

// dsTSTRING<char>::UnsafeInitEmpty @ 0x823B02F8
// Point this (uninitialised) string at the process-wide shared empty-string singleton, lazily
// allocating that singleton (a 0-length buffer) on first use behind a local-static guard and
// bumping its reference count. Does NOT release any prior buffer (the "Unsafe" contract).
template<>
void dsTSTRING<char>::UnsafeInitEmpty()
{
    // Function-local static: the shared empty string, allocated once and registered for
    // destruction at process exit.
    static dsTSTRING<char> emptyStr;
    static bool emptyStrInited; // compiler's `local static guard` bit 0

    if (!emptyStrInited) {
        emptyStr.pBuffer = nullptr;
        emptyStrInited = true;
        emptyStr.pBuffer = emptyStr.AllocBuffer(0, 0);
        // (binary registers a dynamic atexit destructor for emptyStr here)
    }

    this->pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *shared = emptyStr.pBuffer;
    this->pBuffer = emptyStr.pBuffer;
    ++shared->refCount;
}
