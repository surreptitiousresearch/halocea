// ProcessSocketsNotifications @ 0x82792F48  (?ProcessSocketsNotifications@@YAXXZ)
// Poll all socket listeners without blocking, then, under the delayed-list lock and with
// batch-delete mode enabled, destroy every socket queued for deferred deletion and clear the list.

#include "../../headers/ws/ap/ap_sockets_boundary.h"

void ProcessSocketsNotifications()
{
    ap::SOCKET_LISTENER::DoSelectNonBlock();

    gs_batchDelete = 1;
    gs_delayedListLock.Lock(nullptr, 0);

    for (ds::LIST<ap::BASE_SOCKET *>::DATA *node = gs_delayedList.head; node; node = node->next)
    {
        if (node->val)
            node->val->__vftable->dtr(node->val, 1); // virtual deleting destructor
    }
    gs_delayedList.Clear();

    gs_batchDelete = 0;
    gs_delayedListLock.Unlock(nullptr, 0);
}
