#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"

// 0x83231118  aiCOORD_GROUP::cbFilterWPSetByHOvsTargetHODir
// ssl callback: this dispatch forwards the raw (argc, argv) script frame straight to
// FilterWPSetByHOvsTargetHODir (which retrieves its own args), then returns the bool.
void aiCOORD_GROUP::cbFilterWPSetByHOvsTargetHODir(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)caller;

    bool result = this->FilterWPSetByHOvsTargetHODir(argc, argv);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
