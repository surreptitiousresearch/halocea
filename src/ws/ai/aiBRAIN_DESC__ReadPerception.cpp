#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../../headers/ws/ps/psARRAY.h"

// byte_8200155A — the empty parse-error hint literal used to tag the grAvoid* array parses. boundary.
extern const char aiEmptyParseHint[]; // == byte_8200155A ("")

// aiBRAIN_DESC::ReadPerception @ 0x83186D78
// ?ReadPerception@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z
//
// When a "perception" sub-section exists, read the up/down angle-clip flags, peer-update and
// enemy-lock timings, the battle/idle sense descriptors, and the grenade-avoidance radius/delay
// ramps. Note the clip flags and timings are read from the OUTER section, while the sense
// descriptors and ramps come from the "perception" sub-section.
void aiBRAIN_DESC::ReadPerception(const psSECTION &section)
{
    const psSECTION_INTERFACE &ps = static_cast<const psSECTION_INTERFACE &>(section);
    if (!ps.IsKeyExist("perception", 0))
        return;

    psSECTION perception = ps.GetSection("perception", 0);
    const psSECTION_INTERFACE &pi = static_cast<const psSECTION_INTERFACE &>(perception);

    this->isClipUpAngle   = ps.GetBool("angleClipUp", this->isClipUpAngle, 0) != 0;
    this->isClipDownAngle = ps.GetBool("angleClipDown", this->isClipDownAngle, 0) != 0;
    this->timeUpdatePeerOnLF = ps.GetFloat("timeUpdatePeerOnLF", this->timeUpdatePeerOnLF, 0);
    this->timeLockEnemy = ps.GetFloat("timeLockEnemy", this->timeLockEnemy, 0);

    this->battle.ParsePS(perception);
    psSECTION idleSection = pi.GetSection("idle", 0);
    this->idle.ParsePS(idleSection);

    dsTSTRING<char> hint(aiEmptyParseHint);
    psARRAY radiusArray = pi.GetArray("grAvoidRadius", 0);
    this->grAvoidRadius.ParsePS(radiusArray, hint);
    psARRAY delayArray = pi.GetArray("grAvoidDelay", 0);
    this->grAvoidDelay.ParsePS(delayArray, hint);
}
