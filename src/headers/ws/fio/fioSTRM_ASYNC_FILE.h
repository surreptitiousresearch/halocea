#pragma once
#include <stdint.h>
#include "fioFILE.h"
#include "osASYNC_INFO.h"
#include "../ds/dsVECTOR.h"
// ws-engine fio: streaming async pack file. DB-verified layout (types_members
// fioSTRM_ASYNC_FILE, size 548; nested fioSTRM_ASYNC_FILE::BUFFER, size 32776).

struct osASYNC_FILE_HANDLE_DUMMY; // boundary — platform async file handle

struct fioSTRM_ASYNC_FILE : fioFILE {
    // Nested double-buffer record (DB fioSTRM_ASYNC_FILE::BUFFER).
    struct BUFFER {
        int             offset;      /* 0x00 */
        int             size;        /* 0x04 */
        unsigned char   buf[32768];  /* 0x08 */
    };

    osASYNC_FILE_HANDLE_DUMMY *file;          /* 0x020 */
    int                fileSize;              /* 0x024 */
    int                rawOffset;             /* 0x028 */
    int                dataOffset;            /* 0x02C */
    osASYNC_INFO       asyncInfo;             /* 0x030 */
    uint64_t   asyncReadStart;        /* 0x074 */
    struct BUFFER     *buffer1;               /* 0x07C */
    struct BUFFER     *buffer2;               /* 0x080 */
    struct BUFFER     *data;                  /* 0x084 */
    uint8_t   *restBuf;               /* 0x088 */
    dsVECTOR<int, 8>   table;                 /* 0x08C */
    int                compressionType;       /* 0x0A0 DB <unnamed_type_compressionType> enum */
    uint8_t    comprData[384];        /* 0x0A4 */
}; /* 548 bytes */
