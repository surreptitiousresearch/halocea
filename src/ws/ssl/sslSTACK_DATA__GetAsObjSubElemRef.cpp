/* ?GetAsObjSubElemRef@sslSTACK_DATA@@QBA?AVsslERROR@@AAPAVsslOBJECT@@AAPAVsslCLASS@@AAHPAVsslSYSTEM@@@Z @0x82AF0E08 */
#include "sslSTACK_DATA.h"
#include "sslERROR.h"
#include "../ds/dsTSTRING.h"

// Global printf-into-dsTSTRING helper (returns the formatted string by reference); not reversed here.
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// 0x82AF0E08 — sslSTACK_DATA::GetAsObjSubElemRef. On a TYPE_SSLOBJECT_SUBELEM_REF slot, hand back
// the referenced (object, class, sub-element index) and a no-error result; otherwise build a
// formatted conversion error. Returns sslERROR by value (sret): the decompiler swaps this/result
// (r3 = return slot sslERROR*, r4 = this sslSTACK_DATA*). `pSsl` is unused by the reversed body.
sslERROR sslSTACK_DATA::GetAsObjSubElemRef(sslOBJECT *&obj, sslCLASS *&cls, int &idx, sslSYSTEM *pSsl) const
{
    (void)pSsl;

    if (this->type == TYPE_SSLOBJECT_SUBELEM_REF)
    {
        obj = this->subElemRef.pObj;
        cls = this->subElemRef.pObjClass;
        idx = this->subElemRef.idx;
        return sslERROR();
    }

    dsTSTRING<char> msg;
    msg.pBuffer = nullptr;
    // `type` reused as the "%i" arg (the decompiler applies the sslERROR_ID enum to it; it is the
    // stack-data TYPE value that failed the sub-element-ref check).
    return sslERROR(SSL_ERR_GENERIC,
                    *dsSPrintf(&msg, "can't convert stack data %i to subElem ref", this->type),
                    0);
}
