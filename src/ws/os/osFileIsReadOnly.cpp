#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around the CRT write-access check (access(name, 2) == W_OK); the file is
// read-only when the write-access probe FAILS.
int osFileIsReadOnly(const char *fileName)
{
    return access(fileName, 2) != 0;
}
