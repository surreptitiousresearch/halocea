#include "../../headers/ws/rend/rendLIGHT_SET.h"
#include "../../headers/ws/anim/animINST.h"

// 0x82ABBFA8 — clear pInstOwner when the instance being destroyed is the current owner, so
// the light set never outlives a dangling back-pointer to a freed animINST.
void rendLIGHT_SET::NotifyDestroyInst(animINST *pInst)
{
    if (pInst == pInstOwner)
        pInstOwner = nullptr;
}
