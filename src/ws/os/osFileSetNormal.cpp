#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around Win32 SetFileAttributesA(name, FILE_ATTRIBUTE_NORMAL).
int osFileSetNormal(const char *fileName)
{
    return SetFileAttributesA(fileName, 0x80u /* FILE_ATTRIBUTE_NORMAL */) != 0;
}
