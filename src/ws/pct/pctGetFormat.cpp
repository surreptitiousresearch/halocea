/* ?pctGetFormat@@YAPAUpctFORMAT@@H@Z @0x827BFC60 */
#include "../../headers/pctFORMAT.h"
#include "../../headers/pct_format.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// 0x827BFC60 — return the descriptor for a PCT_FORMAT code from the global
// pctFormatList table. Strong-asserts that the code is in (UNKNOWN, MAX) and that
// the table entry has a positive bpp before returning the entry's address.
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp
pctFORMAT *pctGetFormat(unsigned int format)
{
    if (!IGNORE_STRONG_ASSERT)
    {
        if (format < (unsigned int)PCT_FORMAT_MAX
            || (static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                    "format > PCT_FORMAT_UNKNOWN && format < PCT_FORMAT_MAX",
                    "D:\\Projects\\code\\common\\src.sys\\picture\\picture.cpp",
                    2193, empty_string), !IGNORE_STRONG_ASSERT))
        {
            if (pctFormatList[format].bpp <= 0)
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                    "_pctFormatList[format].bpp > 0",
                    "D:\\Projects\\code\\common\\src.sys\\picture\\picture.cpp",
                    2195, empty_string);
        }
    }
    return &pctFormatList[format];
}
