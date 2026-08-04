#include "apMEM_FRAME_DUMPER.h"

// apDlStatFrameNotifyFree @ 0x82C4C2B8
extern apMEM_FRAME_DUMPER _frameDumper;

void apDlStatFrameNotifyFree(int size, const char *filename, int line, apMEM_BLOCK_TYPE type)
{
    _frameDumper.NotifyAp(size, filename, line, type, AP_DLSTAT_DL_FREE);
}
