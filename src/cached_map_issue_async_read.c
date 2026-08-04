#include <stdint.h>
#include "headers/_OVERLAPPED.h"

extern void cached_map_issue_async_request(int (*async_fn)(void *, void *, uint32_t, _OVERLAPPED *, void (*)(uint32_t, uint32_t, _OVERLAPPED *)), void *file, _OVERLAPPED *overlapped, void *buffer, uint32_t size, uint32_t offset, void (*completion)(uint32_t, uint32_t, _OVERLAPPED *));
/* ReadFileEx is a Win32 function; the code passes its address as the read callback */
extern int __stdcall ReadFileEx(void *, void *, unsigned int, _OVERLAPPED *, void *);

void cached_map_issue_async_read(void *file, _OVERLAPPED *overlapped, void *buffer, int size, int offset, void (*completion_routine)(unsigned int, unsigned int, _OVERLAPPED *))
{
    cached_map_issue_async_request(&ReadFileEx, file, overlapped, buffer, size, offset, completion_routine);
}
