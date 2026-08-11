// FUNCTION_INDEX entry: osFileMakeDir @0x825E6C28 (?osFileMakeDir@@YAHPBD@Z)
#include "../../headers/ws/os/os_boundary.h"

// Recursively creates every missing directory component of `fileName`'s path (mkdir -p style).
// Reconstructed from raw pointer-walk decompile; renamed the scratch registers to their
// semantic roles: a local copy of the path is built on the stack, then walked backward to find
// the deepest EXISTING ancestor directory, then walked forward re-inserting path separators and
// calling CreateDirectoryA for each new component.
// CAVEAT: the decompiler's raw indexed stack-copy loop is preserved here as an equivalent
// explicit byte-copy loop into `pathCopy`; the disasm confirms it is a plain strcpy-style copy
// of `fileName` onto the stack buffer, not an overlapping/aliased access.
int osFileMakeDir(const char *fileName)
{
    const char *scan = fileName;
    while (*scan++)
        ;
    int pathLength = (int)(scan - fileName - 1);
    if (scan - fileName == 1) // empty path
        return 0;

    char pathCopy[320];
    {
        const char *src = fileName;
        char c;
        do
        {
            c = *src;
            pathCopy[src - fileName] = *src;
            ++src;
        } while (c);
    }

    // Walk backward from the end to find the deepest ancestor directory that already exists.
    char *cursor = &pathCopy[pathLength - 1];
    if (cursor != pathCopy)
    {
        while (access(pathCopy, 0) != 0)
        {
            while (true)
            {
                char ch = *cursor;
                if (ch == '/' || ch == '\\')
                    break;
                if (--cursor == pathCopy)
                    goto found_existing_prefix;
            }
            *cursor = 0; // truncate at the separator to test the parent directory
        }
    }
found_existing_prefix:
    int existingPrefixLen = (int)(cursor - pathCopy);
    if (existingPrefixLen == pathLength)
        return 1; // the full path already exists

    // Walk forward from the deepest existing ancestor, creating each missing component.
    int createdUpTo = existingPrefixLen;
    int lastCreateDirResult;
    while (true)
    {
        lastCreateDirResult = CreateDirectoryA(pathCopy, nullptr);
        if (createdUpTo != pathLength)
        {
            int scanPos = createdUpTo;
            do
            {
                if (!*cursor)
                    break;
                ++scanPos;
                ++cursor;
            } while (scanPos != pathLength);
        }
        createdUpTo = (int)(cursor - pathCopy);
        if (createdUpTo == pathLength)
            break;
        *cursor = '\\';
    }

    if (lastCreateDirResult)
        return 1;
    // Tolerate ERROR_ALREADY_EXISTS (183) on the final component -- another thread/process may
    // have created it first.
    return GetLastError() == 183;
}
