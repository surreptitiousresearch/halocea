// fnmDIR__FindName @0x828F65D0
#include "../../headers/ws/fnm/fnmDIR.h"
#include "../../headers/ws/fnm/fnm.h"

// fnmDIR::FindName @ 0x828F65E0 -- search this directory (and, on miss, recursively its
// subdirectories and sibling directories) for a file entry. When `ext` is given, `name_ext`
// (the caller's pre-built "name.ext") is looked up directly in nameList via the exact-string
// nameListCMP comparator; otherwise `name` alone is looked up via fnmGetNameCMP, which strips
// extensions off nameList entries before comparing. On a hit, builds this directory's path
// joined with the matched file name into outBuffer (via fnmAddPath) and returns it; returns
// nullptr if nothing matches anywhere in the subtree.
//
// DEVIATION: the raw decompile carries 13 extra trailing int/char* parameters (a6..a18) that are
// never read or written by the function body -- stale stack-slot noise the PPC decompiler
// misattributed as arguments. The mangled name (?FindName@fnmDIR@@QAAPADPBD00PAD@Z) and the
// call site at fnmFindDisc (0x828F6E50) both confirm only 4 real parameters
// (name, ext, name_ext, outBuffer) beyond `this`; the extras are omitted here.
char *fnmDIR::FindName(const char *name, const char *ext, const char *name_ext, char *outBuffer)
{
    if (ext)
    {
        nameListCMP cmp;
        if (nameList.FindSorted<nameListCMP, const char *>(name_ext, cmp) != -1)
            return fnmAddPath(path, name_ext, outBuffer);
    }
    else
    {
        fnmGetNameCMP cmp;
        int idx = nameList.FindSorted<fnmGetNameCMP, const char *>(name, cmp);
        if (idx != -1)
            return fnmAddPath(path, nameList[idx], outBuffer);
    }

    char *result = nullptr;
    if (subDir)
        result = subDir->FindName(name, ext, name_ext, outBuffer);

    if (!result)
    {
        if (!nextDir)
            return nullptr;
        result = nextDir->FindName(name, ext, name_ext, outBuffer);
        if (!result)
            return nullptr;
    }

    return result;
}
