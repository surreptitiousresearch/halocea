#include "msgCMP_ADDR2.h"
#include "msgADDR.h"

// msgCMP_ADDR2::cmp @ 0x825587F8
// Three-way order two priority buckets by their head element's internal priority key. An empty
// bucket sorts before a non-empty one; two non-empty buckets compare their head priorityInt; two
// empty buckets are equal.
unsigned int msgCMP_ADDR2::cmp(const ds::PTR_LIST<msgADDR> *list1, const ds::PTR_LIST<msgADDR> *list2) const
{
    if (!list1->pHead && list2->pHead)
        return (unsigned int)-1;
    if (list1->pHead && !list2->pHead)
        return 1;
    if (list1->pHead || list2->pHead)
        return list1->pHead->priorityInt - list2->pHead->priorityInt;
    return 0;
}
