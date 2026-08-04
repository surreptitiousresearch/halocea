#include "../../headers/ws/os/os_boundary.h"
#include "../../headers/ws/os/osFILE_HANDLE_DUMMY.h"
#include "../../headers/ws/os/OS_FILE.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// Sibling os* free function (src/ws/os/osFileMakeDir.cpp) -- same subsystem, own translation unit.
extern int osFileMakeDir(const char *fileName);

// CAVEAT: the decompile's `mode` parameter is the raw int payload of a dsFLAGS<OS_FILE,int>
// passed by value (confirmed by the mangle `dsFLAGS<enum OS_FILE,int>` on the recursive
// self-call); reconstructed here operating on `mode.val` directly against the DB-verified
// OS_FILE bit values rather than the unreversed dsFLAGS::Test() helper. The tail of the
// decompile also shows a garbled recursive call (`(dsFLAGS<enum OS_FILE,int> *)(mode & ...)`
// stored through an unrelated dsTSTRING buffer-header field slot) -- a register-allocation
// artifact of the value-type-by-register-slot reuse; the real operation is: build a new
// dsFLAGS value with OS_FILE_MAKEDIR cleared and recurse, which is what is reconstructed below.
osFILE_HANDLE_DUMMY *osFileOpen(const char *name, dsFLAGS<OS_FILE, int> mode)
{
    unsigned int desiredAccess = 0;
    unsigned int creationDisposition = 0;

    if (mode.val & OS_FILE_READ)
    {
        if (mode.val & OS_FILE_WRITE)
        {
            desiredAccess = 0xC0000000; // GENERIC_READ | GENERIC_WRITE
            creationDisposition = 4;    // OPEN_ALWAYS
            goto have_access_and_disposition;
        }
        desiredAccess = 0x80000000; // GENERIC_READ
        creationDisposition = 3;    // OPEN_EXISTING
        goto have_access_and_disposition;
    }
    if (mode.val & OS_FILE_WRITE)
    {
        desiredAccess = 0x40000000; // GENERIC_WRITE
        creationDisposition = 2;    // CREATE_ALWAYS
        goto have_access_and_disposition;
    }
    if (mode.val & OS_FILE_APPEND)
    {
        desiredAccess = 0x40000000; // GENERIC_WRITE
        creationDisposition = 4;    // OPEN_ALWAYS
    }
have_access_and_disposition:
    if (mode.val & OS_FILE_CREATE)
        creationDisposition |= 2u;
    if (mode.val & OS_FILE_TRUNCATE)
        creationDisposition |= 5u;

    void *fileHandle = CreateFileA(name, desiredAccess, 1u /* FILE_SHARE_READ */, nullptr,
                                    creationDisposition,
                                    0x08000080u /* FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN */,
                                    nullptr);
    if (fileHandle != (void *)-1)
    {
        if (mode.val & OS_FILE_APPEND)
            SetFilePointer(fileHandle, 0, nullptr, 2u /* FILE_END */);
        return (osFILE_HANDLE_DUMMY *)fileHandle;
    }

    unsigned int lastError = GetLastError();
    if (!(mode.val & OS_FILE_WRITE) || !(mode.val & OS_FILE_MAKEDIR) || lastError != 3 /* ERROR_PATH_NOT_FOUND */)
        return nullptr;

    // The containing directory doesn't exist yet -- create it, then retry once without the
    // MAKEDIR flag (so a second failure doesn't loop).
    dsTSTRING<char> path;
    path.UnsafeInit(name, -1, 0);

    int separatorIndex = path.pBuffer->strLen - 1;
    for (; separatorIndex >= 0; --separatorIndex)
    {
        char ch = path[separatorIndex];
        if (ch == '\\' || ch == '/')
            break;
    }

    if (!separatorIndex)
    {
        // Separator found at (or search collapsed to) index 0 -- no directory component to create.
        return nullptr;
    }

    dsTSTRING<char> directory = path.SubStr(0, separatorIndex);
    path = directory;

    int makeDirResult = osFileMakeDir(path.CStr());
    if (!makeDirResult)
        return nullptr;

    dsFLAGS<OS_FILE, int> retryMode;
    retryMode.val = mode.val & ~OS_FILE_MAKEDIR;
    return osFileOpen(name, retryMode);
}
