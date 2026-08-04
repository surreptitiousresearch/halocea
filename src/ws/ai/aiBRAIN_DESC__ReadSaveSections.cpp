#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"

// aiBRAIN_DESC::ReadSaveSections @ 0x83185F90
// ?ReadSaveSections@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z
//
// Cache the raw "BEHAVIORS" and "APPROACH" sub-section handles for later (re)parsing.
void aiBRAIN_DESC::ReadSaveSections(const psSECTION &section)
{
    const psSECTION_INTERFACE &ps = static_cast<const psSECTION_INTERFACE &>(section);

    if (ps.IsKeyExist("BEHAVIORS", 0))
        this->psBehaviors = ps.GetSection("BEHAVIORS", 0);

    if (ps.IsKeyExist("APPROACH", 0))
        this->psApproach = ps.GetSection("APPROACH", 0);
}
