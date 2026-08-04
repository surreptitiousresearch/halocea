#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/bhvDSC_STORAGE.h"
#include "../../headers/ws/dsc/dscBRAND.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiPLANNER::FindBrand(char*) const @ 0x8316AE50
// Look up a behavior brand in the planner's behavior-descriptor store by its descriptor name
// (null when `nameDesc` is null). The temporary key string is released on return (dsTSTRING dtor).
dscBRAND *aiPLANNER::FindBrand(char *nameDesc) const
{
    if (!nameDesc)
        return nullptr;

    dsTSTRING<char> name;
    name.UnsafeInit(nameDesc, -1, 0);
    return this->bhvDStore->FindBrand(name);
}
