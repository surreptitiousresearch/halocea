#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around the CRT existence check (access(name, 0) == F_OK).
int osFileIsExist(const char *fileName)
{
    return access(fileName, 0) == 0;
}
