#include "msgCMP_INDEX.h"
#include "saLIST.h"

// msgCMP_INDEX::cmp @ 0x82555F20
// Three-way compare keying a subscriber-list entry by its message id.
int msgCMP_INDEX::cmp(const saLIST *list, int msg) const
{
    return list->msg - msg;
}
