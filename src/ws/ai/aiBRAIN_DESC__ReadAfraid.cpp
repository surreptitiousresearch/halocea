#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"

// aiReadVectorStridFromPS @ 0x832D3968
// ?aiReadVectorStridFromPS@@YAXAAV?$dsVECTOR@VdsSTRID@@$07@@PBDABVpsSECTION@@@Z
// Read a list of interned strids stored under `key` into `out`. boundary.
extern void aiReadVectorStridFromPS(dsVECTOR<dsSTRID, 8> &out, const char *key,
                                    const psSECTION &section);

// aiBRAIN_DESC::ReadAfraid @ 0x83185D10
// ?ReadAfraid@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z
//
// Read the "afraid of flying object" reaction data: two squared collision/afraid distances (stored
// as the square of the parsed distance) and four reaction-strid lists.
void aiBRAIN_DESC::ReadAfraid(const psSECTION &section)
{
    const psSECTION_INTERFACE &ps = static_cast<const psSECTION_INTERFACE &>(section);

    if (ps.IsKeyExist("distFlyObjCollision", 0))
    {
        float d = ps.GetFloat("distFlyObjCollision", 0.0f, 0);
        this->distFlyObjCollision_2 = d * d;
    }
    aiReadVectorStridFromPS(this->reactFlyObj_Coll_Right, "reactFlyObj_Coll_Right", section);
    aiReadVectorStridFromPS(this->reactFlyObj_Coll_Left, "reactFlyObj_Coll_Left", section);

    if (ps.IsKeyExist("distFlyObjAfraid", 0))
    {
        float d = ps.GetFloat("distFlyObjAfraid", 0.0f, 0);
        this->distFlyObjAfraid_2 = d * d;
    }
    aiReadVectorStridFromPS(this->reactFlyObj_Afr_Right, "reactFlyObj_Afr_Right", section);
    aiReadVectorStridFromPS(this->reactFlyObj_Afr_Left, "reactFlyObj_Afr_Left", section);
}
