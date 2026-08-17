/* ?_FreeTexCoord@objOBJ@@QAAXH@Z @0x82A81008 */
#include <stdint.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
#include "../ds/dsVECTOR.h"
#include "obj_boundary.h"
// 0x82A81008  ?_FreeTexCoord@objOBJ@@QAAXH@Z
// Release texcoord channel `tcInd` (0..4): free the channel data (unless the channel is shared),
// null out the list, clear the shared flag, then recurse into every geometry-sharing user that also
// has this channel shared so their copies are freed too.
//
// CAVEAT: the decompiler fabricated phantom args and packed the share bit into overlapping slots;
// untangled against the disasm. objGetSharingUsers fills a stack dsVECTOR whose backing store is
// released with dlFree afterwards.

// boundary — obj free function: collect the objects that share geometry with `obj`.
extern void objGetSharingUsers(objOBJ *obj, dsVECTOR<objOBJ *, 8> &users, bool getAllDependentUsers);

void objOBJ::_FreeTexCoord(unsigned int tcInd)
{
    if (!this->pGeom || tcInd > 4)
        return;

    uint64_t shareBit = (uint64_t)0x800 << tcInd; // OBJ_ST_SHARED_TEXCOORD<tcInd>
    bool isShared = (this->stateShare.state & shareBit) != 0;
    if (!isShared)
        FreeObjData(tcInd + 15, GetTexCoordList(tcInd));

    SetTexCoordList(tcInd, nullptr);
    this->stateShare.state &= ~shareBit;

    dsVECTOR<objOBJ *, 8> users;
    users.pData = nullptr;
    users.nElem = 0;
    users.allocated = 0;
    users.__cl.file = "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp";
    users.__cl.line = 735;
    objGetSharingUsers(this, users, true);

    for (int i = 0; i < users.nElem; ++i) {
        objOBJ *user = users[i];
        if ((user->stateShare.state & shareBit) != 0)
            user->_FreeTexCoord(tcInd);
    }

    dlFree(users.pData);
}
