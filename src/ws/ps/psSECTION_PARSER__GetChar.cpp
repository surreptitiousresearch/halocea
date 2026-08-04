#include "psSECTION_PARSER.h"
// 0x82A2C2B0 — pull the next raw character from the reader, advancing `read.pos`. Returns -1 at
// EOF (pos already past end), 0 for the synthetic NUL past the last real byte (pos == end), or the
// unsigned byte value otherwise. Crossing a '\n' advances the line counter and `read.lineStart`.

int psSECTION_PARSER::GetChar()
{
    if (read.pos > read.end)
        return -1;

    if (read.pos == read.end) {
        read.pos = read.pos + 1;
        return 0;
    }

    char c = *read.pos;
    read.pos = read.pos + 1;
    if (c == '\n') {
        read.lineStart = read.pos;
        ++read.lineNmb;
    }
    return (unsigned char)c;
}
