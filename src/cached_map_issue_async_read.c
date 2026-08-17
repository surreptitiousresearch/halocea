/* cached_map_issue_async_read @0x83754388 */
#include <stdint.h>
#include "headers/_OVERLAPPED.h"
#include "headers/win32_async_io_boundary.h"

extern void cached_map_issue_async_request(int (*async_fn)(void *, void *, uint32_t, _OVERLAPPED *, void (*)(uint32_t, uint32_t, _OVERLAPPED *)), void *file, _OVERLAPPED *overlapped, void *buffer, uint32_t size, uint32_t offset, void (*completion)(uint32_t, uint32_t, _OVERLAPPED *));
/* ReadFileEx is a Win32 function; the code passes its address as the read callback. Its declaration
 * lives in headers/win32_async_io_boundary.h — this TU spelled the completion routine `void *`
 * while the other two spelled it as a typed funcptr, which is the divergence that filed this. */

void cached_map_issue_async_read(void *file, _OVERLAPPED *overlapped, void *buffer, int size, int offset, void (*completion_routine)(unsigned int, unsigned int, _OVERLAPPED *))
{
    cached_map_issue_async_request(&ReadFileEx, file, overlapped, buffer, size, offset, completion_routine);
}
