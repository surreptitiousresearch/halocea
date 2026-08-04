#include "psSECTION_PARSER.h"
// 0x82A2C314 — push back `num` characters (walking `read.pos` backward one byte at a time, clamped
// at `read.begin`); crossing a '\n' in reverse decrements the line counter and resets
// `read.lineStart` to just past that newline. After the rewind, if `read.pos` lands back at the
// very start of the buffer, the line counter/lineStart are reset to the beginning-of-buffer state.

void psSECTION_PARSER::UngetChar(int num)
{
    while (num && read.pos > read.begin) {
        --read.pos;
        --num;
        if (*read.pos == '\n') {
            read.lineStart = read.pos + 1;
            --read.lineNmb;
        }
    }

    if (read.pos == read.begin) {
        read.lineStart = read.pos;
        read.lineNmb = 1;
    }
}
