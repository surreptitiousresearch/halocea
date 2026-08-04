#include "apMEM_FRAME_DUMPER.h"

// apDlStatFrameNotifyAlloc @ 0x82C4C290
// The single global per-frame memory-usage logger, notified of every apMEM_BLOCK_TYPE-bucketed
// allocation observed by __apMemoryDebugAdd.
extern apMEM_FRAME_DUMPER _frameDumper;

void apDlStatFrameNotifyAlloc(int size, const char *filename, int line, apMEM_BLOCK_TYPE type)
{
    _frameDumper.NotifyAp(size, filename, line, type, AP_DLSTAT_DL_ALLOC);
}
