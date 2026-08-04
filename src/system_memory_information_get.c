/* system_memory_information_get @0x83762EC8 — query Win32 GlobalMemoryStatus and report free/total
 * physical memory.
 *
 * DEVIATION: the decompiler renders this as a garbled 64-bit HIDWORD/LODWORD read of a QWORD spanning
 * dwTotalPhys/dwAvailPhys; disasm_range(0x83762EC8,0x83762F20) shows two independent 32-bit loads
 * (dwAvailPhys at MEMORYSTATUS+12, dwTotalPhys at MEMORYSTATUS+8) written straight into information->free /
 * ->total (offsets confirmed by the DB's own system_memory_information layout: free@0, total@4); a leading
 * zero-init of both output words is dead code, immediately overwritten. */

#include <windows.h>
#include "headers/system_memory_information.h"

void system_memory_information_get(system_memory_information *information)
{
    MEMORYSTATUS status;

    status.dwLength = sizeof(MEMORYSTATUS);
    GlobalMemoryStatus(&status);
    information->free = status.dwAvailPhys;
    information->total = status.dwTotalPhys;
}
