#include "fioFILE.h"
// fioFILE::Write(const char *) @ 0x825AF580 — write a NUL-terminated string (including
// the terminating NUL) straight to the stream and advance the position.

void fioFILE::Write(const char *s)
{
    const char *scan = s;
    while ( *scan++ )
        ;
    int length = (int)(scan - s); // strlen + 1: scan advanced one past the NUL

    // Decompiler rendered the vtable call as taking only `this`; the DoWriteData slot
    // (vtbl+0x20) really takes (dst-bytes, size) — disasm sets r4=s, r5=length.
    DoWriteData(s, length);

    unsigned int prevStreamSize = streamSize;
    unsigned int newOffset = offset + length;
    offset = newOffset;
    if ( newOffset > prevStreamSize )
        streamSize = newOffset;
}
