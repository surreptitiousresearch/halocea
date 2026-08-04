/* GetMemorySize @ 0x83688B08 — walk the process virtual address space with
 * VirtualQuery from 0 to 0x80000000, summing the RegionSize of every committed
 * (MEM_COMMIT) private (MEM_PRIVATE) region. Returns the total committed private
 * byte count. */

#include "headers/_MEMORY_BASIC_INFORMATION.h"

extern unsigned int VirtualQuery(const void *lpAddress,
                                 MEMORY_BASIC_INFORMATION *lpBuffer,
                                 unsigned int dwLength);

int GetMemorySize(void)
{
    char *address = (char *)0;
    int total = 0;
    MEMORY_BASIC_INFORMATION info;

    do
    {
        VirtualQuery(address, &info, 0x1Cu);
        if ( (info.State & 0x1000) != 0 && info.Type == 0x20000 )
            total += info.RegionSize;
        address += info.RegionSize;
    }
    while ( (unsigned int)address < 0x80000000 );

    return total;
}
