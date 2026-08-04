#include "../../headers/ws/fnm/fnm.h"
#include "../../headers/ws/fnm/fnm_boundary.h"

#include <cctype>
#include <cstring>

// fnmBuildAbsolutePath @ 0x82626040 -- if `name` already looks like a drive-letter absolute path
// ("d:..." / "D:..."), copy it into dst as-is (bounded to 260 chars, matching the sibling
// fnm* functions' MAX_PATH-style buffers); otherwise prefix it with the fixed "D:\\" drive root.
//
// DEVIATION: the raw decompile is `tolower(*(unsigned __int8 *)name == 100)` -- it lowercases the
// *boolean* (name[0]=='d') rather than lowercasing name[0] and comparing to 'd'. That is a
// decompiler misparse of `tolower(name[0]) == 'd'`; reproduced here with the intended comparison.
const char *fnmBuildAbsolutePath(char *dst, const char *name)
{
    if (tolower((unsigned char)name[0]) == 'd' && name[1] == ':')
    {
        strncpy(dst, name, 0x104u);
        dst[259] = 0;
    }
    else
    {
        _snprintf_0(dst, 0x104u, "D:\\%s", name);
    }

    return dst;
}
