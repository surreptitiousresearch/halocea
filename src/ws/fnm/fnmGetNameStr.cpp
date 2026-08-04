#include "../../headers/ws/fnm/fnm.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// fnmGetNameStr @ 0x823C5E08 (?fnmGetNameStr@@YA?AV?$dsTSTRING@D@@ABV1@@Z)
// dsTSTRING wrapper over fnmGetName: build an empty result string, lock a buffer large enough to
// hold the source name, run the raw-string fnmGetName into it, then return it by value.
//
// The compiler expands the default construction as the UnsafeInitEmpty shared-empty-string path
// (seeding pBuffer from the process-wide empty string and ref-bumping it); reproduced here in
// natural by-value form via the default ctor.
dsTSTRING<char> fnmGetNameStr(const dsTSTRING<char> &name)
{
    dsTSTRING<char> result;

    int srcLen = name.pBuffer->strLen;
    char *dst = result.Lock(srcLen + 1);
    fnmGetName(name.pBuffer->str, dst);
    result.Unlock();

    return result;
}
