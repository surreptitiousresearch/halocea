#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiPLANNER::AddParentToSection(psSECTION&, const dsTSTRING<char>&) @ 0x83169DB8
// Look up the behavior-PS section named `secName` in the planner's behavior-section store
// (bhvPSStore) and, when it exists, add it as a parent of `ps`. Returns whether that named
// section existed.
bool aiPLANNER::AddParentToSection(psSECTION &ps, const dsTSTRING<char> &secName)
{
    // bhvPSStore is a psSECTION acting as a psSECTION_INTERFACE (DB: aiPLANNER.bhvPSStore is a
    // psSECTION; the interface derives from it).
    psSECTION_INTERFACE *store = reinterpret_cast<psSECTION_INTERFACE *>(&this->bhvPSStore);

    psSECTION section = store->GetSection(secName.pBuffer->str, 0);
    // A referenced copy is handed to AddParent (which takes psSECTION by value / adopts it).
    reinterpret_cast<psSECTION_INTERFACE &>(ps).AddParent(section);

    return section.pData != nullptr;
}
