#pragma once
// ws-engine fio: open-mode / status bits carried in fioFILE::state (dsFLAGS<FIO_OPEN,int>).
// DB-verified enumerator values (idapython enum dump of FIO_OPEN).

enum FIO_OPEN {
    FIO_OPEN_R              = 0x1,   // open for reading
    FIO_OPEN_W             = 0x2,   // open for writing
    FIO_OPEN_A             = 0x4,   // open for append
    FIO_OPEN_CHUNK          = 0x8,   // chunked stream (descrList carries chunk offsets)
    FIO_OPEN_NO_OFFSET_END  = 0x10,  // do not back-patch chunk end offsets on Term
    FIO_OPEN_BYTE_REORDER   = 0x20,  // endian byte-swap on read/write
    FIO_ERROR               = 0x40,  // a stream error occurred
    FIO_ERROR_EOF           = 0x80,  // end of stream reached
    FIO_OPEN_ALIGN_SECTOR   = 0x100, // sector-aligned (dlReallocAligned) backing buffer
    FIO_OPEN_USER           = 0x200, // caller-defined flag
};
