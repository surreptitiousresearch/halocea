#pragma once
#include "fioFILE.h"
#include "../ds/dsFLAGS.h"
#include "FIO_OPEN.h"
// ws-engine fio: disk-backed stream. DB-verified layout (types_members fioFILE_DISK):
// fioFILE base @0 (size 32), then the disk-specific fields — size 316.

struct osFILE_HANDLE_DUMMY; // boundary — opaque OS file handle

// Callback type consulted before opening a disk file (may veto the open).
typedef int (*fioFILE_DISK_OPEN_CB)(const char *fileName, int state, int bufSize);

struct fioFILE_DISK : fioFILE {
    char                  fileName[260];    // 0x20 open path
    osFILE_HANDLE_DUMMY  *fp;               // 0x124 OS file handle (null = closed)
    int                   attrWasChanged;   // 0x128 file attributes were modified
    unsigned int          dwFileAttributes; // 0x12C cached Win32 file attributes
    char                 *buffer;           // 0x130 write-behind buffer
    int                   bufSize;          // 0x134 bytes pending in `buffer`
    int                   bufSizeMax;       // 0x138 capacity of `buffer`

    // Process-wide open veto hook (?openCb@fioFILE_DISK@@..); null = always allow.
    static fioFILE_DISK_OPEN_CB openCb;

    // --- reversed in this batch ---
    int Term();                       // 0x825B05B0 (overrides fioFILE::Term)
    void FlushData();                         // 0x825AD2A0
    int  InitFile(const char *fileName, dsFLAGS<FIO_OPEN,int> state, int bufSize); // 0x825AD0E8

    fioFILE_DISK();                    // 0x825AD090 default ctor
    ~fioFILE_DISK();           // 0x825B07B8 (overrides fioFILE::~fioFILE)
};
