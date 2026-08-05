#pragma once
#include "fioFILE.h"
#include "osASYNC_INFO.h"
#include "../ds/dsFLAGS.h"
#include "FIO_OPEN.h"
// ws-engine fio: memory-backed stream. DB-verified layout (types_members fioFILE_MEM):
// fioFILE base @0 (size 32), then the memory-specific fields — size 124.

struct osASYNC_FILE_HANDLE_DUMMY; // boundary — opaque async OS file handle

struct fioFILE_MEM : fioFILE {
    unsigned char             *pBuf;             // 0x20 backing memory block
    unsigned char             *curPtr;           // 0x24 current cursor into pBuf
    unsigned int               allocSize;        // 0x28 allocated size of pBuf
    int                        isSelfManaged;    // 0x2C pBuf owned by this stream
    int                        predictAllocSize; // 0x30 growth prediction hint
    osASYNC_FILE_HANDLE_DUMMY *fpAsync;          // 0x34 async source handle (streamed init)
    osASYNC_INFO               fpAsyncInfo;      // 0x38 async completion record (size 68)

    // --- reversed in this batch ---
    int Term();                          // 0x825B05F8 (overrides fioFILE::Term)
    void TermMemory();                           // 0x825AD930
    int  InitMemory(void *buffer, unsigned int buflen, dsFLAGS<FIO_OPEN,int> state);   // 0x825AE7E0
    int  InitAllocate(unsigned int buflen, dsFLAGS<FIO_OPEN,int> state);               // 0x825AE8F0
    // Read the named file fully into the backing buffer; returns nonzero on success. boundary
    // (called from package::GS_PACKAGE::ProcessLoadTextures; body external to this batch).
    int  ReadFile(const char *path);

    fioFILE_MEM();                  // 0x825AD550 default ctor
    ~fioFILE_MEM();         // 0x825AE730 (overrides fioFILE::~fioFILE)

    // Extra vtable slots beyond fioFILE's 10 (types_members fioFILE_MEM_vtbl: GetBuf@40,
    // GetCurPtr@44) — bodies not decompiled here.
    unsigned char *GetBuf();    // slot 10 boundary
    unsigned char *GetCurPtr(); // slot 11 boundary
};
