#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"

// aiBRAIN_DESC::ReadOther @ 0x83185CC0
// ?ReadOther@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z
//
// Read the single "forceSetupInitial" flag (defaulting to its current value when absent).
void aiBRAIN_DESC::ReadOther(const psSECTION &section)
{
    const psSECTION_INTERFACE &ps = static_cast<const psSECTION_INTERFACE &>(section);
    this->forceSetupInitial = ps.GetBool("forceSetupInitial", this->forceSetupInitial, 0) != 0;
}
