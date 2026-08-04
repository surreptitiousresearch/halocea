#include "../../headers/ws/txm/txmTEXTURE.h"
#include "../../headers/ws/txm/txmMANAGER.h"

// 0x826F59C8 -- ?MarkAsUsedInRender@txmTEXTURE@@QAAXXZ
// Stamp this texture with the manager's current frame number, marking it as touched this frame
// for the streaming manager's used-vs-idle bookkeeping.
void txmTEXTURE::MarkAsUsedInRender()
{
    this->frameNumUpd = txmManager->currFrame;
}
