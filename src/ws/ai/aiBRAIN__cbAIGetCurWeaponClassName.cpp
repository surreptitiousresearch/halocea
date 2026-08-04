#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::cbAIGetCurWeaponClassName(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324AF78
// ?cbAIGetCurWeaponClassName@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "AIGetCurWeaponClassName" getter callback: store the brain's current weapon class name
// (implAIGetCurWeaponClassName) into `retVal`. `caller` is passed by value and released on return.
void aiBRAIN::cbAIGetCurWeaponClassName(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    retVal.StoreValue<dsTSTRING<char> >(implAIGetCurWeaponClassName());
    // the implAIGetCurWeaponClassName() temporary and the caller ref release themselves at scope end.
}
