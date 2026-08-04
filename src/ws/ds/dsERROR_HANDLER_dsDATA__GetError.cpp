#include "dsERROR_HANDLER.h"
#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

struct dsDATA; // boundary — ds data object, opaque here

// dsERROR_HANDLER<dsDATA>::GetError @ 0x82ADA288
// If `instance` currently owns a non-empty error, move that error string out of the channel
// (bumping then clearing the shared slot so ownership transfers to the result) and clear the
// owner; otherwise return an empty string. Returns dsTSTRING<char> by value (sret in r3).
template<>
dsTSTRING<char> dsERROR_HANDLER<dsDATA>::GetError(const dsDATA *instance)
{
    dsTSTRING<char> result;

    bool hasError = true;
    if (GetCurrentInst() != instance || GetErrorStr().pBuffer->strLen == 0)
        hasError = false;

    if (hasError) {
        // Transfer the shared error buffer into the result, then clear the channel.
        dsTSTRING_BUF_HEADER<char> *buffer = GetErrorStr().pBuffer;
        ++buffer->refCount;
        SetCurrentInst(nullptr);
        GetErrorStr().Clear();
        int refCount = buffer->refCount;
        result.pBuffer = buffer;
        buffer->refCount = refCount + 1;
        buffer->refCount = refCount;
        if (refCount == 0)
            dlFree(buffer);
    } else {
        result.pBuffer = nullptr;
        result.UnsafeInitEmpty(); // inlined shared-empty-string singleton in the original
    }
    return result;
}
