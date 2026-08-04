#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiCOORD_GROUP::cbFilterWPSetByMinHeight @ 0x83231658
// ?cbFilterWPSetByMinHeight@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: read a minimum height from argv[0], dispatch FilterWPSetByMinHeight, store the bool.
void aiCOORD_GROUP::cbFilterWPSetByMinHeight(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    float minHeight = 0.0f;
    argv->GetValue<float>(minHeight, DSD_CONV_RETRIEVE);

    bool result = this->FilterWPSetByMinHeight(minHeight);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
