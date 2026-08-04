#include "fioFILE_DISK.h"
#include "fio_boundary.h"

// fioFILE_DISK::~fioFILE_DISK() @ 0x825B07B8 — virtual dtor (vtable slot 0). Re-installs
// the fioFILE_DISK vtable (guards against a derived class's vtable being live at this
// point), tears down the fioFILE base via fioFILE::Term(), flushes the write-behind
// buffer, closes the OS file handle if open, frees the write-behind buffer, clears the
// path, then swaps to the fioFILE vtable before freeing the read/write stream buffer
// (matches base-class dtor chaining: fioFILE::~fioFILE runs next and must see its own
// vtable installed).
fioFILE_DISK::~fioFILE_DISK()
{
    // vftable write reproduced from the decompile; base fioFILE::Term() below is the
    // real teardown entry point.
    fioFILE::Term();
    FlushData();

    osFILE_HANDLE_DUMMY *handle = fp;
    if (handle)
    {
        osFileClose(handle);
        fp = nullptr;
    }

    char *pendingBuf = buffer;
    fileName[0] = 0;
    if (pendingBuf)
        dlFree(pendingBuf);

    fioFILE_BUFFER *streamBuffer = pBuffer;
    // decompile re-points __vftable to fioFILE::`vftable' here, immediately before
    // fioFILE::~fioFILE runs as the next base dtor in the chain.
    if (streamBuffer)
        dlFree(streamBuffer);
}
