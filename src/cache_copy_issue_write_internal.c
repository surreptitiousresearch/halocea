/* cache_copy_issue_write_internal @0x838088B8 — start an async write from a copy-worker write buffer:
 * mark its overlapped slot (write buffers occupy slots 9+) in-flight, stash the slot index in the
 * OVERLAPPED's hEvent (unused by WriteFileEx) and the file offset, then issue WriteFileEx, spinning
 * alertably (SleepEx(0,1)) and retrying while the OS reports a transient no-resources error
 * (ERROR_INVALID_USER_BUFFER / ERROR_NOT_ENOUGH_MEMORY / ERROR_WORKING_SET_QUOTA). Any other failure sets
 * the worker's error flag. */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"
#include "headers/blam_data_globals.h"


extern void cache_copy_FileIOCompletionRoutine(unsigned int dwErrorCode, unsigned int dwNumberOfBytesTransfered, _OVERLAPPED *lpOverlapped);

/* Win32 boundary */
extern unsigned int SleepEx(unsigned int milliseconds, int alertable);
extern void SetLastError(unsigned int error);
extern unsigned int GetLastError(void);
extern int WriteFileEx(void *file, const void *buffer, unsigned int bytes_to_write, _OVERLAPPED *overlapped,
        void (*completion_routine)(unsigned int, unsigned int, _OVERLAPPED *));

void cache_copy_issue_write_internal(simple_decompressor_definition *self, void *buffer, int size, int offset, int16_t write_buffer_index)
{
    void *write_file_handle = self->write_file_handle;
    int slot = write_buffer_index + 9;
    _OVERLAPPED *overlapped = &self->overlapped[slot];

    self->overlapped_in_use_flags[0] |= 1 << (slot & 0x1F);
    overlapped->hEvent = (void *)(intptr_t)slot;
    overlapped->Offset = offset;
    overlapped->OffsetHigh = 0;

    while ( 1 )
    {
        SleepEx(0, 1);
        SetLastError(0);
        int issued = WriteFileEx(write_file_handle, buffer, size, overlapped,
                                 cache_copy_FileIOCompletionRoutine);
        unsigned int last_error = GetLastError();
        if ( issued )
            break;
        if ( last_error != 1784 && last_error != 8 && last_error != 1450 )
        {
            global_self->flags |= 1u;
            return;
        }
    }
}
