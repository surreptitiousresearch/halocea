#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
#include "sslCLASS.h"
#include "sslERROR.h"
#include "../ds/dsTSTRING.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x82523D88 — sslOBJ_REF::HandleEvent. Dispatch event `idx` on the referenced object via its class,
// forwarding (argc, argv). When this ref is empty (object <null>) build a formatted error instead.
// Returns sslERROR by value (sret): the decompiler swaps this/result (r3 = return slot sslERROR*,
// r4 = this sslOBJ_REF*). sslCLASS::HandleEvent is a re-source boundary one level past this method.
sslERROR sslOBJ_REF::HandleEvent(int idx, int argc, dsDATA *const *argv)
{
    if (this->pObject)
    {
        return this->pObject->refClass.pClass->HandleEvent(this->pObject, idx, argc, argv);
    }

    dsTSTRING<char> msg;
    return sslERROR(SSL_ERR_GENERIC,
                    *dsSPrintf(&msg, "handle event idx'%i' on the <null> object", idx),
                    0);
}
