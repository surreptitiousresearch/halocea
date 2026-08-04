#include "fioFILE_DISK.h"

// fioFILE_DISK::fioFILE_DISK() @ 0x825AD090 — default ctor. Zeroes the fioFILE base
// (offset/streamSize/chunk-descr list/state), installs the fioFILE_DISK vtable, then
// zeroes the disk-specific fields (OS handle, attribute cache, write-behind buffer) and
// the open path.
fioFILE_DISK::fioFILE_DISK()
{
    offset = 0;
    streamSize = 0;
    nDescr = 0;
    nDescrAlloc = 0;
    descrList = nullptr;
    state.val = 0;
    pBuffer = nullptr;
    fp = nullptr;
    attrWasChanged = 0;
    dwFileAttributes = 0;
    buffer = nullptr;
    bufSize = 0;
    bufSizeMax = 0;
    fileName[0] = 0;
}
