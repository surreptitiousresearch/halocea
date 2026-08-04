#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/prop/propGAME_INFO.h"

// 0x832ABB88 ?GetMinHeightFlesh@aiBRAIN@@UBAMXZ
// Min flesh height above feet: from the status peeker watcher when present, else the game-info default.
float aiBRAIN::GetMinHeightFlesh() const
{
    aiSTATUS *status = this->st.pointee;
    aiWATCHER_BRAIN *peeker;
    if (status && (peeker = status->peekerSelf) != nullptr)
        return peeker->GetMinHeightFlesh();

    return this->propGameInfo()->GetMinHeightFlesh();
}
