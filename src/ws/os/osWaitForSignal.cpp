// FUNCTION_INDEX entry: osWaitForSignal @0x825E6150 (?osWaitForSignal@@YAHPAUosHANDLE_DUMMY@@K@Z)
#include "../../headers/ws/os/os_boundary.h"
#include "../../headers/ws/os/osHANDLE_DUMMY.h"

// Thin wrapper around Win32 WaitForSingleObject(). Maps WAIT_TIMEOUT (258) to 256 and any
// non-WAIT_OBJECT_0 result to -1 (0 on success, -1 on any other failure code), matching the
// disasm's `(waitResult == 0) - 1` idiom.
int osWaitForSignal(osHANDLE_DUMMY *handle, unsigned long msTimeout)
{
    unsigned int waitResult = WaitForSingleObject(handle, msTimeout);
    if (waitResult == 258 /* WAIT_TIMEOUT */)
        return 256;
    return (waitResult == 0) - 1; // 0 on WAIT_OBJECT_0, -1 otherwise
}
