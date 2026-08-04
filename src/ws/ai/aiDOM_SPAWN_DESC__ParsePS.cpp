#include "../../headers/ws/ai/aiDOM_SPAWN_DESC.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"

// aiDOM_SPAWN_DESC::ParsePS @ 0x83263F00
// ?ParsePS@aiDOM_SPAWN_DESC@@UAAXVpsSECTION@@ABV?$dsTSTRING@D@@@Z
//
// Parse the base spawn-domain descriptor, then capture the nested "OPP_PS" (opponent-spawn) section
// into oppPs. `psRead` is taken by value (a referenced section copy); the base call and GetSection
// both operate on that copy. The gsDOM_SPAWN_DESC base sits at offset 0, so the base call goes
// through a reinterpret_cast of `this`.
void aiDOM_SPAWN_DESC::ParsePS(psSECTION psRead, const dsTSTRING<char> &hintErr)
{
    reinterpret_cast<gsDOM_SPAWN_DESC *>(this)->ParsePS(psRead, hintErr);

    const psSECTION_INTERFACE &ps = static_cast<const psSECTION_INTERFACE &>(psRead);
    this->oppPs = ps.GetSection("OPP_PS", 0);
}
