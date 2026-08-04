#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/prop/propGAME_INFO.h"

// 0x832ABBF8 ?GetHeight@aiBRAIN@@UBAMXZ
// Full body height: from the status peeker watcher when present, else the game-info default.
float aiBRAIN::GetHeight() const
{
    aiSTATUS *status = this->st.pointee;
    aiWATCHER_BRAIN *peeker;
    if (status && (peeker = status->peekerSelf) != nullptr)
        return peeker->GetHeight();

    return this->propGameInfo()->GetHeight();
}
