#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslCLASS_REF.h"
#include "../../headers/ws/ssl/sslCLASS.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/gs/gsGAME_TIMER.h"

// aiBRAIN::RemoveSSLCrossRefs @ 0x83179790
// ?RemoveSSLCrossRefs@aiBRAIN@@MAAXXZ
//
// On teardown, walk every variable (data member) of this brain's SSL object. For each that holds a
// gsGAME_TIMER reference, detach its expiry callback so nothing fires against a destroyed brain.
void aiBRAIN::RemoveSSLCrossRefs()
{
    sslCLASS *pClass = sslObject.GetClass().pClass; // temp sslCLASS_REF released here
    if (!pClass)
        return;

    int nElem = pClass->elements.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        if (!pClass->IsSubElemVar(i))
            continue;

        dsDATA data;
        data.type = nullptr;
        sslERROR err = sslObject.GetVar(i, data);
        if (err.id == SSL_ERR_NO_ERROR)
        {
            ds_data::REF_TYPE<gsGAME_TIMER> ref;
            if (data.GetValue(ref, DSD_CONV_RETRIEVE))
                ref.m_pData->m_pT->ClearCB();
        }
        // `data` (and `err`'s desc string) are released by their scoped destructors — matching the
        // decompiler's per-iteration type->Destroy / refcount-drop cleanup.
    }
}
