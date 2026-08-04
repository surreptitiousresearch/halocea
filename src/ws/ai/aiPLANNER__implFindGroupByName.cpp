#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiPLANNER::implFindGroupByName @ 0x8322ECB8
// ?implFindGroupByName@aiPLANNER@@UAA?AVsslOBJ_REF@@ABV?$dsTSTRING@D@@@Z
//
// Look up a coordinated group by name through the embedded coordinator (aiPLANNER::coordinator
// @0x184) and return its SSL object reference, or an empty reference when no such group exists.
sslOBJ_REF aiPLANNER::implFindGroupByName(const dsTSTRING<char> &grpName)
{
    aiCOORDINATOR *coord = (&coordinator);
    aiCOORD_GROUP *group = coord->GetGroup_3(&grpName);
    if (group)
        return sslOBJ_REF(*group->GetSslObj());
    return sslOBJ_REF();
}
