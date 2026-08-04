#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::DropWeapon() @ 0x83249688
// ?DropWeapon@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "DropWeapon" SSL event to drop the brain's weapon (no arguments, no return value).
void aiBRAIN::DropWeapon()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnDropWeapon(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
