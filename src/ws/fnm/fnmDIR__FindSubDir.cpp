// fnmDIR__FindSubDir @0x828F3FE8
#include "../../headers/ws/fnm/fnmDIR.h"

#include <cstring>
#include <cctype>

// fnmDIR::FindSubDir @ 0x828F3FF8 -- search this directory (and, on miss, recursively its
// subdirectories and sibling directories) for a subdirectory whose full path matches `_path`.
// A match is either: (a) `this->path` case-insensitively equals `_path` in full, or (b) `_path`
// begins with `this->path` and the next character in `_path` is a path delimiter (i.e. `_path`
// names something located inside this directory).
//
// DEVIATION: the raw decompile open-codes a manual byte-by-byte case-sensitive comparison
// (computing `v10` as the signed byte difference) for check (b) after already using stricmp() for
// check (a) -- an inlined strncmp idiom over `strlen(this->path)` bytes. Reproduced with
// strncmp for clarity; behavior (byte-exact match required) is unchanged.
fnmDIR *fnmDIR::FindSubDir(const char *_path)
{
    int pathLen = (int)strlen(path);

    if (stricmp(path, _path) == 0)
        return this;

    if (strncmp(path, _path, pathLen) == 0)
    {
        int matchLen = (int)strlen(_path);
        if (matchLen == pathLen + 1)
        {
            char nextChar = _path[pathLen];
            if (nextChar == '\\' || nextChar == '/')
                return this;
        }
    }

    fnmDIR *result = nullptr;
    if (subDir)
        result = subDir->FindSubDir(_path);

    if (!result)
    {
        if (!nextDir)
            return nullptr;
        result = nextDir->FindSubDir(_path);
        if (!result)
            return nullptr;
    }

    return result;
}
