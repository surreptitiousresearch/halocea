/* ?pctIsCompressed@@YAHH@Z @0x827C0338 */
#include "../../headers/pctFORMAT.h"
#include "../../headers/pct_format.h"
#include "../../headers/pct_boundary.h"

// 0x827C0338 — is a PCT_FORMAT code a block-compressed (DXTn) format? Reads bit 3
// of the format descriptor's `state` field; out-of-range codes report false.
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp
unsigned int pctIsCompressed(unsigned int format)
{
    if (format > (unsigned int)(PCT_FORMAT_MAX - 1))
        return 0;
    return ((unsigned int)pctGetFormat(format)->state >> 3) & 1;
}
