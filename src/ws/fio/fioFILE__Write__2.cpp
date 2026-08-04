#include "fioFILE.h"
#include "../ds/dsTSTRING.h"
// fioFILE::Write(const dsTSTRING<char> &) @ 0x825AFD18 — write a length-prefixed string:
// a 4-byte character count (byte-reordered) followed by the raw characters.
// Overload #2 (higher address than Write(const char*) @ 0x825AF580).

void fioFILE::Write(const dsTSTRING<char> &str)
{
    int charCount = str.pBuffer->strLen;
    WriteData(&charCount, 4, 4);

    char *chars = str.pBuffer->str;
    int length = str.pBuffer->strLen;
    // DoWriteData slot takes (bytes, size); decompiler kept both args here.
    DoWriteData(chars, length);

    unsigned int prevStreamSize = streamSize;
    unsigned int newOffset = offset + length;
    offset = newOffset;
    if ( newOffset > prevStreamSize )
        streamSize = newOffset;
}
