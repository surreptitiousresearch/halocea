#include "../../headers/ws/fnm/fnm.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// fnmGetNameExtStr @ 0x823C5C78 (?fnmGetNameExtStr@@YA?AV?$dsTSTRING@D@@ABV1@@Z)
// dsTSTRING wrapper over fnmGetNameExt (keeps the extension): build an empty result string, lock a
// buffer sized to the source name, run fnmGetNameExt into it, then return it by value. Identical
// in shape to fnmGetNameStr, differing only in the raw helper it drives.
dsTSTRING<char> fnmGetNameExtStr(const dsTSTRING<char> &name)
{
    dsTSTRING<char> result;

    int srcLen = name.pBuffer->strLen;
    char *dst = result.Lock(srcLen + 1);
    fnmGetNameExt(name.pBuffer->str, dst);
    result.Unlock();

    return result;
}
