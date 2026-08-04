#include <string.h>
#include "headers/_LARGE_INTEGER.h"
#include "headers/blam_data_globals.h"

int QueryPerformanceFrequency(_LARGE_INTEGER *lpFrequency);


void cseries_platform_initialize(void)
{
    QueryPerformanceFrequency(&s_ClockFrequency);
    memset(scratch_disk_root, 0, sizeof(scratch_disk_root));
    /* void return: r3 at blr (0x8376317C) is strncpy residue; no caller consumes it */
    strncpy(scratch_disk_root, "d:", 0x104u);
}
