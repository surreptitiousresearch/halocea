#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ent/entENTITY.h"

// aiGRASS::RemoveFlyingGrenade @ 0x83184A80
// Walk the tracked-grenades list and erase every cell whose grenade entity matches `grenade`.
void aiGRASS::RemoveFlyingGrenade(const entENTITY *grenade)
{
    ds::LIST<GRENADE_INFO>::DATA *node = this->grenades.head;
    while (node)
    {
        if (node->val.grenade == grenade)
        {
            ds::LIST<GRENADE_INFO>::ITERATOR where;
            where.data = node;
            node = this->grenades.Erase(where).data;
        }
        else
        {
            node = node->next;
        }
    }
}
