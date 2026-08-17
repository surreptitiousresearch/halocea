/* ?FindClass@sslSYSTEM@@QAA?AVsslCLASS_REF@@VdsSTRID@@@Z @0x825CEB38 */
#include "sslSYSTEM.h"
#include "sslCLASS_REF.h"
#include "sslCLASS_MAP_ELEM.h"
#include "../ds/dsSTRID.h"

// boundary — stateless comparator over interned name ids (compare inlined in FindSorted).
struct sslCMP_NAMES {};

// 0x825CEB38 — sslSYSTEM::FindClass. Binary-search the name-sorted class map for `name`; return a
// ref to the class (empty on miss). Returns sslCLASS_REF by value (sret): the decompiler swaps
// this/result (r3 = return slot sslCLASS_REF*, r4 = this sslSYSTEM*). `name` is passed by value.
sslCLASS_REF sslSYSTEM::FindClass(dsSTRID name)
{
    sslCMP_NAMES cmp;
    int idx = this->namedClasses.FindSorted<sslCMP_NAMES, dsSTRID>(name, cmp);
    if (idx == -1)
        return sslCLASS_REF();
    return this->namedClasses[idx].cls;
}
