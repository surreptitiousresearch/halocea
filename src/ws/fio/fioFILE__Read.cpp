#include "fioFILE.h"
// fioFILE::Read(char *, int) @ 0x825ACC28 — read a NUL-terminated string from the stream.
// With maxLen > 0 it copies bytes into `str` up to maxLen, always NUL-terminating the
// final slot; if the string is longer than the buffer, the remaining characters are
// still drained from the stream (up to the NUL or EOF) so the read position stays in
// sync. With maxLen <= 0 it drains-and-discards the string.

void fioFILE::Read(char *str, int maxLen)
{
    int outPos = 0;
    unsigned char scratch[48];
    unsigned char lastByte;

    if ( maxLen > 0 )
    {
        while ( 1 )
        {
            ReadDataImpl(scratch, 1, 1);
            lastByte = scratch[0];
            bool isNul = scratch[0] == 0;
            str[outPos] = (char)scratch[0];
            if ( isNul )
                break;
            if ( ++outPos >= maxLen )
                goto terminate_and_drain; // buffer full but string continues
        }
        return;
    }

    lastByte = scratch[0]; // maxLen <= 0: scratch is unread here (matches decompile)

terminate_and_drain:
    str[maxLen - 1] = 0;
    if ( lastByte )
    {
        do
        {
            if ( IsEOF() )
                break;
            ReadDataImpl(scratch, 1, 1);
        }
        while ( scratch[0] );
    }
}
