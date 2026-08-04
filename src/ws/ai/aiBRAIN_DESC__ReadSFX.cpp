#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"

// aiBRAIN_DESC::ReadSFX @ 0x83185F18
// ?ReadSFX@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z
//
// When present, read the "on_screen_marker" sub-section and pull its "index" into idxOnScreen.
void aiBRAIN_DESC::ReadSFX(const psSECTION &section)
{
    const psSECTION_INTERFACE &ps = static_cast<const psSECTION_INTERFACE &>(section);

    if (ps.IsKeyExist("on_screen_marker", 0))
    {
        psSECTION marker = ps.GetSection("on_screen_marker", 0);
        const psSECTION_INTERFACE &mi = static_cast<const psSECTION_INTERFACE &>(marker);
        this->idxOnScreen = mi.GetInt("index", this->idxOnScreen, 0);
    }
}
