#include "sslSYSTEM.h"
#include "sslOBJ_REF.h"
#include "sslERROR.h"
#include "../ds/dsTSTRING.h"

// boundary — stateless comparator over interned name ids (compare inlined in FindSorted).
struct sslCMP_NAMES {};

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// @0x825CE368 — sslSYSTEM::FindObject (by-name overload __2). Returns sslOBJ_REF by value (sret): the
// decompiler swaps this/result (r3 = return slot sslOBJ_REF*, r4 = this sslSYSTEM*). Reset lastErr,
// binary-search the name-sorted object table; on a miss record an SSL_ERR_UNKNOWN_NAME error and
// return an empty ref.
sslOBJ_REF sslSYSTEM::FindObject(const dsTSTRING<char> &name)
{
    this->lastErr = sslERROR(); // clear any prior error to no-error

    sslCMP_NAMES cmp;
    int idx = this->nameToObject.FindSorted<sslCMP_NAMES, dsTSTRING<char> >(name, cmp);
    if (idx == -1)
    {
        dsTSTRING<char> msg;
        this->lastErr = sslERROR(SSL_ERR_UNKNOWN_NAME,
                                 *dsSPrintf(&msg, "unknown object name '%s'", name.pBuffer->str),
                                 0);
        return sslOBJ_REF();
    }
    return this->nameToObject[idx];
}
