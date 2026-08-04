#include <stdint.h>
#include "headers/_OVERLAPPED.h"

uint32_t SleepEx(uint32_t ms, int alertable);
void     SetLastError(uint32_t error);
uint32_t GetLastError(void);

void cached_map_issue_async_request(
        int (*async_fn)(void *, void *, uint32_t, _OVERLAPPED *, void (*)(uint32_t, uint32_t, _OVERLAPPED *)),
        void *file,
        _OVERLAPPED *overlapped,
        void *buffer,
        uint32_t size,
        uint32_t offset,
        void (*completion)(uint32_t, uint32_t, _OVERLAPPED *))
{
    overlapped->Internal     = 0;
    overlapped->InternalHigh = 0;
    overlapped->Offset       = 0;
    overlapped->OffsetHigh   = 0;
    overlapped->hEvent       = 0;
    overlapped->Offset       = offset;
    overlapped->OffsetHigh   = 0;
    overlapped->hEvent       = 0;

    SleepEx(0, 1);
    SetLastError(0);
    while (!async_fn(file, buffer, size, overlapped, completion))
    {
        GetLastError();
        SleepEx(0, 1);
        SetLastError(0);
    }
}
