#include "../../headers/ws/fnm/fnm.h"

#include <cstring>

// fnmAddPath @ 0x82624FD0 -- copy `path` into outBuffer, ensure it ends with exactly one '\\'
// path delimiter, then append `name`.
//
// DEVIATION: the decompiler renders the trailing-delimiter check and the final append as manual
// byte-by-byte copy loops to the string's NUL terminator -- the classic inlined
// strcpy/strcat idiom for this compiler. Reproduced with strcpy/strcat, which is semantically
// identical.
char *fnmAddPath(const char *path, const char *name, char *outBuffer)
{
    strcpy(outBuffer, path);

    int length = (int)strlen(outBuffer);
    if (length > 0)
    {
        char lastChar = outBuffer[length - 1];
        if (lastChar != '\\' && lastChar != '/')
        {
            outBuffer[length] = '\\';
            outBuffer[length + 1] = 0;
        }
    }

    strcat(outBuffer, name);
    return outBuffer;
}
