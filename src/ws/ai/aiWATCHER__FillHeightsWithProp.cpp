// aiWATCHER::FillHeightsWithProp @ 0x8328D718
// ?FillHeightsWithProp@aiWATCHER@@IAAXXZ
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/prop/propGAME_INFO.h"

// Refresh the cached height fields from the tracked game-info block: body height is 0.9x the
// game-info height (default 0.1 with no game info), minimum flesh height comes straight from the
// game info (default 0.2).
void aiWATCHER::FillHeightsWithProp()
{
    propGAME_INFO *gameInfo = pGameInfo;
    height = gameInfo ? (gameInfo->GetHeight() * 0.89999998f) : 0.1f;

    gameInfo = pGameInfo;
    minHeightFlesh = gameInfo ? gameInfo->GetMinHeightFlesh() : 0.2f;
}
