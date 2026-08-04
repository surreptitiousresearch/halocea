#include "apMEM_FRAME_DUMPER.h"
#include "apMEM_DL_STAT.h"
#include "../ds/dsCONST_ARRAY.h"

// apMEM_FRAME_DUMPER::FreeDl @ 0x82C4C148
void apMEM_FRAME_DUMPER::FreeDl(int size)
{
    if (!this->isStarted)
        return;
    if (this->dlStat.nEntry >= 32)
        return;

    this->sizeDl.size -= size;

    apMEM_DL_STAT entry;
    entry.size = size;
    entry.type = AP_DLSTAT_DL_FREE;
    this->dlStat.Add(entry);
    this->dlStat.Back();
}
