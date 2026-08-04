#include "apMEM_FRAME_DUMPER.h"

// Boundary rodata: apMEM_DL_ALLOC_TYPE -> label string, indexed by allocType. This is the
// global `name` @0x8420BB48 (a small const char*[] of "free"/"alloc" style labels).
extern const char *const apMemApStatTypeName[];

// apMEM_FRAME_DUMPER::NotifyAp @ 0x82C4BF30 — private (AAA mangle).
// Fold an ap-allocator alloc/free into the running/peak ap totals and log it, but only while the
// dumper is started and the event falls inside the watched bucket + size window; anything else
// routes to DumpDlInfo to drain the matching dl-snapshot entries.
void apMEM_FRAME_DUMPER::NotifyAp(int memSize, const char *filename, int fileLine,
                                  apMEM_BLOCK_TYPE _type, apMEM_DL_ALLOC_TYPE allocType)
{
    if (!this->isStarted)
        return;

    if (memSize >= this->size && this->type == _type)
    {
        if (allocType)   // AP_DLSTAT_DL_ALLOC
        {
            int newSize = this->sizeAp.size + memSize;
            this->sizeAp.size = newSize;
            if (newSize > this->sizeAp.sizeMax)
                this->sizeAp.sizeMax = newSize;
        }
        else             // AP_DLSTAT_DL_FREE
        {
            this->sizeAp.size -= memSize;
        }

        this->Dump(this->nameLog, (char *)"%s, %10d, %20s, %10d, %10d\n",
                   apMemApStatTypeName[allocType], memSize, filename, fileLine, this->sizeAp.size);
    }
    else
    {
        this->DumpDlInfo(allocType);
    }
}
