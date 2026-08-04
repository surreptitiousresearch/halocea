#include "../../headers/ws/fnm/fnm.h"
#include "../../headers/ws/fnm/fnm_boundary.h"
#include "../../headers/ws/fnm/fnmDIR.h"

#include <cstring>

// fnmFindDisc @ 0x828F6E50 -- resolve `path_` to its cached directory-tree node (searching every
// root in fnmDirList[0..fnmNDir)), then look up `name`(.`ext`) inside that directory tree via
// fnmDIR::FindName. Asserts strlen(name) < 128 unless IGNORE_STRONG_ASSERT.
//
// DEVIATION: like fnmDIR::FindName itself, the raw decompile's call to FindName carries a batch
// of unread stack-slot int/char* args (v14..v26) tacked on after the real 4 (name, ext,
// name_ext, outBuffer) -- decompiler noise, omitted here (see fnmDIR__FindName.cpp).
char *fnmFindDisc(const char *path_, const char *name, const char *ext, char *outBuffer)
{
    char resolvedPath[336];
    fnmResolvePath(resolvedPath, path_);

    if (fnmNDir <= 0)
        return nullptr;

    fnmDIR *matchedDir = nullptr;
    for (int i = 0; i < fnmNDir; ++i)
    {
        matchedDir = fnmDirList[i]->FindSubDir(resolvedPath);
        if (matchedDir)
            break;
    }
    if (!matchedDir)
        return nullptr;

    if (!IGNORE_STRONG_ASSERT)
    {
        if (strlen(name) >= 64 * 2)
            STRONG_ASSERT_DUMMY::Crash(nullptr, "strlen(name) < 64 * 2", "D:\\Projects\\code\\common\\src.sys\\fileio\\fdir.cpp", 892, g_assertContextByte);
    }

    const char *nameWithExt = fnmAddExt(name, ext, outBuffer);
    char nameExtBuf[128];
    strncpy(nameExtBuf, nameWithExt, 0x80u);

    return matchedDir->FindName(name, ext, nameExtBuf, outBuffer);
}
