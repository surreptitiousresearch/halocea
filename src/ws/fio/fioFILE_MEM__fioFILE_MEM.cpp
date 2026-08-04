#include "fioFILE_MEM.h"

// fioFILE_MEM::fioFILE_MEM() @ 0x825AD550 — default ctor. Zeroes the fioFILE base,
// installs the fioFILE_MEM vtable, then defaults the memory-stream fields: self-managed
// ownership defaults ON, predictAllocSize defaults to 100000 bytes, and the async
// completion record (fpAsyncInfo) is cleared field-by-field (matching the decompile,
// which stores to `signal` twice — once mid-sequence, once at the end — both zero).
fioFILE_MEM::fioFILE_MEM()
{
    offset = 0;
    streamSize = 0;
    nDescr = 0;
    nDescrAlloc = 0;
    descrList = nullptr;
    state.val = 0;
    pBuffer = nullptr;
    pBuf = nullptr;
    curPtr = nullptr;
    allocSize = 0;
    isSelfManaged = 1;
    predictAllocSize = 100000;
    fpAsync = nullptr;
    fpAsyncInfo.isEOF = 0;
    fpAsyncInfo.signal = nullptr;
    fpAsyncInfo.isSignaled = 0;
    fpAsyncInfo.mustRetry = 0;
    fpAsyncInfo.holdSignal = 0;
    fpAsyncInfo.signal = nullptr; // decompile stores nullptr to `signal` a second time
}
