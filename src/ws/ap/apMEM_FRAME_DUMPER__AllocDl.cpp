#include "apMEM_FRAME_DUMPER.h"
#include "apMEM_DL_STAT.h"
#include "../ds/dsCONST_ARRAY.h"

// apMEM_FRAME_DUMPER::AllocDl @ 0x82C4C0C8
void apMEM_FRAME_DUMPER::AllocDl(int size)
{
    if (!this->isStarted)
        return;
    if (this->dlStat.nEntry >= 32)
        return;

    int newSize = this->sizeDl.size + size;
    this->sizeDl.size = newSize;
    if (newSize > this->sizeDl.sizeMax)
        this->sizeDl.sizeMax = newSize;

    apMEM_DL_STAT entry;
    entry.size = size;
    entry.type = AP_DLSTAT_DL_ALLOC;
    this->dlStat.Add(entry);
    this->dlStat.Back();
}
