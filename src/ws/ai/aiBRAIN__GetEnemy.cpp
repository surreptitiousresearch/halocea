#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslCB_HOST.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsDATA.h"

struct sslSYSTEM;
extern sslSYSTEM *gsSslSystem; // global ssl script system (haloInit_boundary.h)

// aiBRAIN::GetEnemy() @ 0x83249948
// ?GetEnemy@aiBRAIN@@QAAPAViaIACTOR@@XZ
//
// Run the descriptor's "GetEnemy" SSL function (no args); its return value is a script object
// reference to the current enemy actor. Resolve that reference against the global script system and
// return the referenced object's callback host (null when the reference is empty).
iaIACTOR *aiBRAIN::GetEnemy()
{
    dsDATA ret;
    ret.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnGetEnemy(spDesc.pointee), 0, nullptr, &ret, sslOBJ_REF());

    sslOBJ_REF enemyObj;
    enemyObj.Set(ret, gsSslSystem); // resolve the returned reference against the script system

    // GetCbHost yields the referenced object's callback host; the enemy actor IS that host (the
    // DB mangle types the result iaIACTOR*). Same pointer value, reinterpreted to the actor type.
    sslCB_HOST *host = enemyObj.GetCbHost();
    return reinterpret_cast<iaIACTOR *>(host);
    // enemyObj and ret destructors self-release at scope end.
}
