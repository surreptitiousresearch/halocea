#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiWATCHER.h"

// aiGRASS::DropAll @ 0x831849A8
// ?DropAll@aiGRASS@@QAAXXZ
//
// Clear all three tracking lists. The victim watchers are owned, so each is destroyed before its
// list cell is released.
void aiGRASS::DropAll()
{
    this->grenInits.Clear();
    this->grenades.Clear();
    for (ds::LIST<aiWATCHER *>::DATA *node = this->victims.head; node; node = node->next)
        aiWATCHER::Destroy(node->val);
    this->victims.Clear();
}
