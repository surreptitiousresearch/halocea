#include "../../headers/ws/fnm/fnm.h"

#include <cstring>

// fnmGetName @ 0x82625580 (?fnmGetName@@YAPADPBDPAD@Z) -- extract `name`'s filename (last path
// component) into outBuffer with its extension removed. fnmGetNameExt does the path-stripping
// copy; then the last '.' (if any) is turned into a NUL to drop the extension. Returns the
// (stripped) filename buffer.
char *fnmGetName(const char *name, char *outBuffer)
{
    char *fileName = fnmGetNameExt(name, outBuffer); // returns writable outBuffer
    char *dot = strrchr(fileName, '.');
    if (dot)
        *dot = 0;
    return fileName;
}
