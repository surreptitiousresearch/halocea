#pragma once
// ws-engine fio: read-ahead / write-behind stream buffer header, DB-verified layout
// (types_members fioFILE_BUFFER): size 20 with a trailing flexible payload.

typedef struct fioFILE_BUFFER {
    unsigned int bufSizeCur;   // 0x00 bytes currently usable in `data`
    unsigned int bufSizeMax;   // 0x04 allocated capacity of `data`
    unsigned int bufFileOfs;   // 0x08 stream offset that `data[0]` maps to
    unsigned int bufDataLen;   // 0x0C valid byte count held in `data`
    unsigned char data[1];     // 0x10 flexible payload (bufSizeMax bytes)
} fioFILE_BUFFER;
