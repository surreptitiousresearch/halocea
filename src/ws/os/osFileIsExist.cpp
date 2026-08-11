// FUNCTION_INDEX entry: osFileIsExist @0x825E5958 (?osFileIsExist@@YAHPBD@Z)
#include "../../headers/ws/os/os_boundary.h"

// Thin wrapper around the CRT existence check (access(name, 0) == F_OK).
int osFileIsExist(const char *fileName)
{
    return access(fileName, 0) == 0;
}
