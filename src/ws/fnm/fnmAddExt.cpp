#include "../../headers/ws/fnm/fnm.h"

#include <cstring>

// fnmAddExt @ 0x82625098 -- copy `name` into outBuffer, then either replace its existing
// extension with `ext`, strip it (ext == nullptr), or append `.ext` if none was found.
//
// DEVIATION: the decompiler renders the tail-append (no existing extension) and the extension
// substitution as manual byte-by-byte copy loops walking to the string's NUL terminator -- the
// classic inlined strcpy/strcat/strcat-to-offset idiom for this compiler. Reproduced with
// strcpy/strcat, which is semantically identical.
char *fnmAddExt(const char *name, const char *ext, char *outBuffer)
{
    strcpy(outBuffer, name);

    // Scan backward from the end of outBuffer for the last path delimiter.
    int nameLen = (int)strlen(outBuffer);
    int lastDelimIndex;
    for (lastDelimIndex = nameLen - 1; lastDelimIndex >= 0; --lastDelimIndex)
    {
        char c = outBuffer[lastDelimIndex];
        if (c == '\\' || c == '/')
            break;
    }

    char *dot = strrchr(outBuffer, '.');
    if (dot && dot >= &outBuffer[lastDelimIndex + 1])
    {
        // An extension already exists past the last path delimiter.
        if (ext)
        {
            if (*ext == '.')
                ++ext;
            strcpy(dot + 1, ext);
        }
        else
        {
            *dot = 0;
        }
    }
    else if (ext)
    {
        // No existing extension -- append one.
        if (*ext != '.')
            strcat(outBuffer, ".");
        strcat(outBuffer, ext);
    }

    return outBuffer;
}
