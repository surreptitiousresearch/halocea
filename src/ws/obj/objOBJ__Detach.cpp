#include "objOBJ.h"
#include "../../headers/animINST.h"
// 0x826605C8  ?Detach@objOBJ@@QAAXXZ
// Unlink this object from its sibling ring and parent, detach it from any animated instance, and
// reset it to a self-referencing singleton (next==prev==this, no parent/instance).

void objOBJ::Detach()
{
    if (this->prev == this) {
        // STRONG_ASSERT: next == this  (Object.cpp:235)
        objOBJ *parent = this->parent;
        if (parent)
            parent->child = nullptr;
    } else {
        // STRONG_ASSERT: !(next != this && parent == 0)  (Object.cpp:244)
        this->next->prev = this->prev;
        this->prev->next = this->next;
        objOBJ *parent = this->parent;
        if (parent && parent->child == this)
            parent->child = this->next;
    }

    animINST *pInst = this->pInst;
    if (pInst)
        pInst->DetachObj(this);

    this->prev = this;
    this->next = this;
    this->parent = nullptr;
    this->pInst = nullptr;
    this->animNmb = -1;
}
