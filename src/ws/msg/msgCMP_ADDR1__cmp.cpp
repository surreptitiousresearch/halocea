#include "msgCMP_ADDR1.h"
#include "msgADDR.h"

// msgCMP_ADDR1::cmp @ 0x82558790
// Order a priority-sorted address list by comparing its head element's internal priority key
// against pAddr's. An empty list sorts before any address (returns -1).
unsigned int msgCMP_ADDR1::cmp(const ds::PTR_LIST<msgADDR> *list, msgADDR *pAddr) const
{
    if (list->pHead)
        return list->pHead->priorityInt - pAddr->priorityInt;
    return (unsigned int)-1;
}
