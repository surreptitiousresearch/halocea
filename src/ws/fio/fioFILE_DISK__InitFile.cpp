#include "fioFILE_DISK.h"
#include "fio_boundary.h"
#include <string.h> // strcpy
// fioFILE_DISK::InitFile(const char *, dsFLAGS<FIO_OPEN,int>, int) @ 0x825AD0E8 —
// open a disk file. Consults the optional openCb veto, records the path/flags (always
// forcing FIO_OPEN_BYTE_REORDER for on-disk endian handling), sizes the write buffer,
// derives the OS open mode from the FIO flags, then opens the handle. On any failure it
// tears the stream back down and returns 0; on success (reading) it caches the file size.

int fioFILE_DISK::InitFile(const char *fileName_, dsFLAGS<FIO_OPEN,int> state_, int bufSize_)
{
    if ( openCb && !openCb(fileName_, state_.val, bufSize_) )
    {
        _apLog("~File~File '%s' open blocked", fileName_);
        return 0;
    }

    strcpy(fileName, fileName_);

    fioCHUNK_DESCR *chunkList = descrList;
    streamSize = 0;
    state.val = state_.val | FIO_OPEN_BYTE_REORDER; // always endian-swap on disk (|0x20)
    offset = 0;
    if ( chunkList )
        dlFree(chunkList);
    nDescr = 0;
    nDescrAlloc = 0;
    descrList = nullptr;
    SetBuffer(bufSize_);

    unsigned int flags = state.val;
    int osOpenMode = 0;
    if ( flags & FIO_OPEN_R )
    {
        osOpenMode = (flags & FIO_OPEN_W) ? 3   // read + write
                                          : 1;  // read only
    }
    else if ( flags & FIO_OPEN_A )
    {
        osOpenMode = 36; // append
    }
    else if ( flags & FIO_OPEN_W )
    {
        osOpenMode = 42; // write / create
    }

    osFILE_HANDLE_DUMMY *handle;
    // osFileOpen's real 2nd arg is dsFLAGS<OS_FILE,int> by value; decompiler cast the
    // 4-byte mode word to a pointer. Passing the raw mode value here.
    if ( fileName_[0] == 0
      || (handle = osFileOpen(fileName_, osOpenMode), (fp = handle) == nullptr) )
    {
        Term();
        return 0;
    }

    if ( (state.val & FIO_OPEN_R) != 0 )
        streamSize = osFileGetSize(handle);
    return 1;
}
