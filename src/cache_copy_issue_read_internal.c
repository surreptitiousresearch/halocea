/* cache_copy_issue_read_internal @0x838087E0 — issue one async ReadFileEx for a cache-copy read buffer,
 * retrying while the OS reports a transient failure (ERROR_IO_PENDING-adjacent busy codes), and marking
 * the copy as failed on any other error. The buffer index is stashed in the OVERLAPPED's (otherwise-unused
 * by ReadFileEx) hEvent slot so the completion routine can identify which buffer finished. */

#include <stdint.h>
#include "headers/win32_async_io_boundary.h"
#include "headers/simple_decompressor_definition.h"
#include "headers/copy_flags.h"
#include "headers/blam_data_globals.h"


extern unsigned int SleepEx(unsigned int dwMilliseconds, int bAlertable);
extern void         SetLastError(unsigned int dwErrCode);
extern unsigned int GetLastError(void);
extern void cache_copy_FileIOCompletionRoutine(unsigned int dwErrorCode, unsigned int dwNumberOfBytesTransfered, _OVERLAPPED *lpOverlapped);

void cache_copy_issue_read_internal(simple_decompressor_definition *self, void *buffer, unsigned int size, int offset, int16_t read_buffer_index)
{
    void         *read_file_handle = self->read_file_handle;
    _OVERLAPPED  *overlapped       = &self->overlapped[read_buffer_index];

    self->overlapped_in_use_flags[0] |= 1 << (read_buffer_index & 0x1F);

    overlapped->hEvent     = (void *)(intptr_t)read_buffer_index;
    overlapped->Offset     = offset;
    overlapped->OffsetHigh = 0;

    while (1)
    {
        SleepEx(0, 1);
        SetLastError(0);

        int ok = ReadFileEx(read_file_handle, buffer, size, overlapped, cache_copy_FileIOCompletionRoutine);
        unsigned int last_error = GetLastError();
        if (ok)
            break;

        if (last_error != 1784 && last_error != 8 && last_error != 1450)
        {
            global_self->flags |= 1u << _copy_read_failed_bit;
            return;
        }
    }
}
