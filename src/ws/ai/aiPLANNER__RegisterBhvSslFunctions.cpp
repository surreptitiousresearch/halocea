#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/bhvDSC_STORAGE.h"
#include "../../headers/ws/dsc/dscBRAND.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslCLASS_REF.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiPLANNER::RegisterBhvSslFunctions(sslCLASS_REF&) @ 0x8316E540
// Register every behavior brand's SSL script functions with `sslClass` (each brand's owned
// descriptor exposes a RegisterSslFunctions virtual; skip brands whose descriptor is null).
void aiPLANNER::RegisterBhvSslFunctions(sslCLASS_REF &sslClass)
{
    dsVECTOR<dscBRAND *, 8> &brands = this->bhvDStore->brandList;
    for (int i = 0; i < brands.nElem; ++i)
    {
        dscDESC *desc = brands[i]->spDesc.pointee;
        if (desc)
            desc->__vftable->RegisterSslFunctions(desc, &sslClass);
    }
}
