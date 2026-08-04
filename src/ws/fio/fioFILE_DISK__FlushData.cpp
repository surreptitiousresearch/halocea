#include "fioFILE_DISK.h"
#include "fio_boundary.h"
// fioFILE_DISK::FlushData() @ 0x825AD2A0 — flush the write-behind buffer to the OS file.
// A short write latches FIO_ERROR. Empties the pending count on success (or no handle).

void fioFILE_DISK::FlushData()
{
    int pending = bufSize;
    if ( pending )
    {
        char *pendingBuf = buffer;
        if ( pendingBuf )
        {
            osFILE_HANDLE_DUMMY *handle = fp;
            if ( handle )
            {
                if ( osFileWrite(handle, pendingBuf, pending) != bufSize )
                    state.val |= FIO_ERROR; // 0x40
            }
            bufSize = 0;
        }
    }
}
