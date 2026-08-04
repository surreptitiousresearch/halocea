#include "apDATA_TRACKER.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free

// apDATA_TRACKER::RECORD::operator= @ 0x82A24498
// Copy-assign a tracker record: unless self-assignment, drop the current name buffer and adopt
// (share, refcount++) the source's, then copy the 30-float value array element by element.
apDATA_TRACKER::RECORD &apDATA_TRACKER::RECORD::operator=(const apDATA_TRACKER::RECORD &other)
{
    if (&other != this)
    {
        dsTSTRING_BUF_HEADER<char> *oldBuffer = this->name.pBuffer;
        if (oldBuffer->refCount-- == 1)
            dlFree(oldBuffer);

        this->name.pBuffer = other.name.pBuffer;
        ++this->name.pBuffer->refCount;
    }

    for (int i = 0; i < 30; ++i)
        this->values[i] = other.values[i];

    return *this;
}
