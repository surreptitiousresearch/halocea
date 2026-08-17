/* system_milliseconds @0x83762BD0 */
#include <stdint.h>
#include "headers/_LARGE_INTEGER.h"
#include "headers/blam_data_globals.h"

int QueryPerformanceFrequency(_LARGE_INTEGER *frequency);
int QueryPerformanceCounter(_LARGE_INTEGER *count);


uint32_t system_milliseconds(void)
{
    _LARGE_INTEGER counter;

    /* disasm 0x83762BE8 ld + cmpdi: full 64-bit QuadPart test, not LowPart */
    if (!s_ClockFrequency.QuadPart)
        QueryPerformanceFrequency(&s_ClockFrequency);
    QueryPerformanceCounter(&counter);
    return (unsigned int)((counter.QuadPart * 1000) / s_ClockFrequency.QuadPart);
}
