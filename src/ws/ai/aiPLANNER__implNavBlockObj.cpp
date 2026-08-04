// aiPLANNER::implNavBlockObj @ 0x83247850
// ?implNavBlockObj@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@0@Z
//
// SSL-exposed op: block the nav graph around the named sub-object `objName` of scene instance
// `instName`. Resolves the instance, then drives the SMR (spawn-movement-restrict) helper with its
// isEnabledBySSL gate forced on for the duration of the call. Returns true when the instance exists.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPHYS_SMR.h"
#include "../../headers/animINST.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// scnSCENE::FindInstName(gsScenePtr, name): resolve a scene instance by name (scene boundary).
extern animINST *scnFindInstName(const char *name);

bool aiPLANNER::implNavBlockObj(const dsTSTRING<char> &instName, const dsTSTRING<char> &objName)
{
    animINST *inst = scnFindInstName(instName.pBuffer->str);
    if (!inst)
        return false;

    aiPHYS_SMR *smr = (&this->smr);
    smr->isEnabledBySSL = true;
    smr->ProcessAddObjVsNav(inst, objName, 4u, 8u);
    smr->isEnabledBySSL = false;
    return true;
}
