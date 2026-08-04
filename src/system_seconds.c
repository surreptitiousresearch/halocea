#include <stdint.h>
#include "headers/_LARGE_INTEGER.h"
#include "headers/blam_data_globals.h"

int QueryPerformanceFrequency(_LARGE_INTEGER *frequency);
int QueryPerformanceCounter(_LARGE_INTEGER *count);


uint32_t system_seconds(void)
{
    _LARGE_INTEGER counter;

    /* disasm 0x83762B90 ld + cmpdi: full 64-bit QuadPart test, not LowPart */
    if (!s_ClockFrequency.QuadPart)
        QueryPerformanceFrequency(&s_ClockFrequency);
    QueryPerformanceCounter(&counter);
    return (unsigned int)(counter.QuadPart / s_ClockFrequency.QuadPart);
}
