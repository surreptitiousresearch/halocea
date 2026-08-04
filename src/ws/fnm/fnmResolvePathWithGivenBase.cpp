#include "../../headers/ws/fnm/fnm.h"
#include "../../headers/ws/fnm/fnm_boundary.h"

#include <cstring>

// fnmResolvePathWithGivenBase @ 0x82626268 -- if `name` starts with "./" or ".\\", splice
// (name+2) onto the end of dst's existing contents (adding a '/' delimiter first if dst doesn't
// already end with one). Otherwise overwrite dst with `name` verbatim. Asserts the final
// strlen(dst) < 260 unless IGNORE_STRONG_ASSERT.
//
// DEVIATION: the raw decompile writes the "else" branch (plain name -> dst copy) via pointer
// arithmetic on the *source* pointer (`v3[dst - name] = *v3`), which is just an obfuscated
// strcpy(dst, name) -- the constant offset (dst - name) turns every write into dst[i] = name[i].
// Reproduced as strcpy. Likewise the "append name+2" path's manual copy loops are the classic
// inlined strcat idiom, reproduced with strcat.
char *fnmResolvePathWithGivenBase(char *dst, const char *name)
{
    bool isDotRelative = (name[0] == '.') && (name[1] == '\\' || name[1] == '/');

    if (isDotRelative)
    {
        int dstLen = (int)strlen(dst);
        if (dstLen > 0)
        {
            char lastChar = dst[dstLen - 1];
            if (lastChar != '\\' && lastChar != '/')
                strcat(dst, "/");
        }
        strcat(dst, name + 2);
    }
    else
    {
        strcpy(dst, name);
    }

    if (!IGNORE_STRONG_ASSERT)
    {
        if (strlen(dst) >= 0x104)
            STRONG_ASSERT_DUMMY::Crash(nullptr, "strlen(dst) < 260", "D:\\Projects\\code\\common\\src.sys\\fileio\\fname.cpp", 795, g_assertContextByte);
    }

    return dst;
}
