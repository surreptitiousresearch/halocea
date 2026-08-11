#include "fioFILE_MEM.h"
#include "fio_boundary.h"
// fioFILE_MEM::InitAllocate(unsigned int, dsFLAGS<FIO_OPEN,int>) @ 0x825AE8F0 — set up a
// self-managed, growable write buffer of `buflen` bytes. Only valid for a writable stream
// (asserts otherwise). Grows the backing block via the aligned/plain debug reallocator per
// FIO_OPEN_ALIGN_SECTOR, or frees it for a zero length. Returns 1 on success.

int fioFILE_MEM::InitAllocate(unsigned int buflen, dsFLAGS<FIO_OPEN,int> state_)
{
    static const char *const kFileIoCpp =
        "D:\\Projects\\code\\common\\src.sys\\fileio\\fileio.cpp";

    if ( (state_.val & FIO_OPEN_W) == 0 )
    {
        if ( !IGNORE_STRONG_ASSERT )
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("0", kFileIoCpp, 1957, empty_string);
        return 0;
    }

    if ( !isSelfManaged )
        TermMemory();

    fioCHUNK_DESCR *chunkList = descrList;
    state.val = state_.val;
    streamSize = 0;
    offset = 0;
    if ( chunkList )
        dlFree(chunkList);
    nDescr = 0;
    nDescrAlloc = 0;
    descrList = nullptr;
    SetBuffer(0);

    bool isEmpty = (buflen == 0);
    if ( (int)buflen > 0 )
    {
        if ( allocSize < buflen )
        {
            unsigned char *oldBlock = pBuf;
            unsigned char *newBlock;
            if ( (state.val & FIO_OPEN_ALIGN_SECTOR) != 0 )
                newBlock = (unsigned char *)dlReallocAligned(
                    oldBlock, buflen, 0x2000u, kFileIoCpp, 0x7B5u);
            else
                newBlock = (unsigned char *)dlRealloc(
                    oldBlock, buflen, kFileIoCpp, 0x7B7u);
            pBuf = newBlock;
            if ( !newBlock )
            {
                state.val |= FIO_ERROR; // 0x40
                return 0;
            }
            allocSize = buflen;
            goto finish;
        }
        isEmpty = (buflen == 0);
    }
    if ( isEmpty )
    {
        dlFree(pBuf);
        pBuf = nullptr;
        allocSize = 0;
    }

finish:
    unsigned char *finalBlock = pBuf;
    streamSize = 0;
    isSelfManaged = 1;
    offset = 0;
    curPtr = finalBlock;
    return 1;
}
