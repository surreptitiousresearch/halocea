#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiWATCHER.h"

// aiGRASS::ProcessSmokingOut @ 0x83183398
// Walk the victims list; any watcher that is no longer valid, or has dropped out of hideout
// mode, is destroyed and unlinked. Watchers still valid and in hideout mode are kept.
void aiGRASS::ProcessSmokingOut()
{
    ds::LIST<aiWATCHER*>::DATA *node = this->victims.head;
    while (node)
    {
        aiWATCHER *w = node->val;
        if (w && w->IsValid() && w->IsHideoutMode())
        {
            node = node->next;
            continue;
        }

        ds::LIST<aiWATCHER*>::DATA *dead = node;
        ds::LIST<aiWATCHER*>::DATA *next = node->next;
        aiWATCHER::Destroy(w);

        if (dead->prev)
            dead->prev->next = next;
        else
            this->victims.head = next;

        if (next)
            next->prev = dead->prev;
        else
            this->victims.tail = dead->prev;

        operator delete(dead);
        --this->victims.length;
        node = next;
    }
}
