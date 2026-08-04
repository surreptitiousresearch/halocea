#include "animINST.h"

// Free function that registers this instance against its root object; boundary — not reversed here.
int animSetObjInstRef(objOBJ *pObj, animINST *pInst); // boundary

// animINST::SetObjInstRef @ 0x825E2E90
int animINST::SetObjInstRef()
{
    return animSetObjInstRef(this->pObj, this);
}
