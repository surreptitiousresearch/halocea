/* ?SetInstOwner@rendLIGHT_SET@@QAAXPAVanimINST@@@Z @0x82ABBFF0 */
#include "../../headers/ws/rend/rendLIGHT_SET.h"
#include "../../headers/ws/anim/animINST.h"

// 0x82ABBFF0 — set the owning instance back-reference; when detaching (pInst == nullptr)
// also clear isOwnerInst since there is no longer a live instance owner.
void rendLIGHT_SET::SetInstOwner(animINST *pInst)
{
    pInstOwner = pInst;
    if (!pInst)
        isOwnerInst = false;
}
