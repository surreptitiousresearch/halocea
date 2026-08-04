// aiCHEAP_VISION::DropAll @ 0x8323FE30
// ?DropAll@aiCHEAP_VISION@@QAAXXZ
#include "../../headers/ws/ai/aiCHEAP_VISION.h"
#include <new>

// Free every cached VIS_INFO_ENEMY (clearing its sighting list first) and empty listVIE.
// NOTE: the original clears the node's listVI and raw-frees the node (global operator delete, no
// destructor call) — reproduced verbatim.
void aiCHEAP_VISION::DropAll()
{
    for (ds::LIST<aiCHEAVIS::VIS_INFO_ENEMY *>::DATA *node = listVIE.head; node; node = node->next)
    {
        aiCHEAVIS::VIS_INFO_ENEMY *val = node->val;
        if (val)
        {
            val->listVI.Clear();
            ::operator delete(val);
        }
    }
    listVIE.Clear();
}
