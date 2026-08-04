/* cache_copy_FileIOCompletionRoutine @0x83808690 — Win32 overlapped-I/O completion callback for the async
 * map-copy worker. lpOverlapped identifies which of the 11 read/write slots (index = its position within
 * global_self->overlapped[]) finished. On error, sets the appropriate error flag bit (bit 0 for the write
 * slot, index 9; bit 1 for any read slot). On success, marks that slot as no-longer-in-use and completed;
 * slot 9 (the async write) decrements async_write_bytes_left, while any read slot (0-7) additionally
 * recomputes read_progress and signals/resets the progress event.
 *
 * DEVIATION: the decompiler renders the progress computation as `__int64`-punned register reuse
 * (`(float)v10 / (float)v9`); disasm_range(0x83808710, 0x83808768) shows it is plainly
 * `(float)(header.size - async_read_bytes_left) / (float)header.size`, with `progress_update_event`
 * (loaded once, before ResetEvent) reused unchanged for the trailing SetEvent call. */

#include "headers/simple_decompressor_definition.h"

/* Win32 imports (kernel32 boundary; system <windows.h> collides with the repo Win32 shim types
 * pulled in via simple_decompressor_definition.h -> _OVERLAPPED.h). */
extern int ResetEvent(void *hEvent);
extern int SetEvent(void *hEvent);

void cache_copy_FileIOCompletionRoutine(unsigned int dwErrorCode, unsigned int dwNumberOfBytesTransfered,
        _OVERLAPPED *lpOverlapped)
{
    simple_decompressor_definition *self = global_self;
    unsigned int *overlapped_completed_flags = self->overlapped_completed_flags;
    int slot_index = ((char *)lpOverlapped - (char *)self - 2464) / 20;

    if (dwErrorCode)
    {
        self->flags |= (slot_index == 9) ? 1 : 2;
        return;
    }

    if (slot_index < 0)
    {
        if (slot_index == 9)
            self->async_write_bytes_left -= dwNumberOfBytesTransfered;
        return;
    }

    if (slot_index < 11)
    {
        int word_index = slot_index >> 5;
        unsigned int bit = 1 << (slot_index & 0x1F);
        self->overlapped_in_use_flags[word_index] &= ~bit;
        overlapped_completed_flags[word_index] |= bit;
    }

    if (slot_index > 7)
    {
        if (slot_index == 9)
            self->async_write_bytes_left -= dwNumberOfBytesTransfered;
        return;
    }

    void *progress_update_event = self->progress_update_event;
    self->async_read_bytes_left -= dwNumberOfBytesTransfered;
    ResetEvent(progress_update_event);

    int size = self->header.size;
    int bytes_done = size - self->async_read_bytes_left;
    self->read_progress = (float)bytes_done / (float)size;
    SetEvent(progress_update_event);
}
