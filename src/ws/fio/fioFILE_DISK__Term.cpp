#include "fioFILE_DISK.h"
#include "fio_boundary.h"
// fioFILE_DISK::Term() @ 0x825B05B0 (vtable slot 2 override) — run the base finalizer,
// flush any pending writes, close the OS handle, and clear the path.

int fioFILE_DISK::Term()
{
    int baseResult = fioFILE::Term();
    FlushData();
    osFILE_HANDLE_DUMMY *handle = fp;
    if ( handle )
    {
        osFileClose(handle);
        fp = nullptr;
    }
    fileName[0] = 0;
    return baseResult;
}
