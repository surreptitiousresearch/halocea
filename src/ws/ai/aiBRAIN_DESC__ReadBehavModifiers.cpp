#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include <math.h>

// aiBRAIN_DESC::ReadBehavModifiers @ 0x83186490
// ?ReadBehavModifiers@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z
//
// Read the optional "bhvModifiers" sub-section: grenade/tower fear radii, smart-death toggle,
// several 3D-hard-obstacle (HO) tuning flags/distances, the big-turn path angle (stored as its
// cosine), yield/reach distances, the moral archetype and melee-speed name, and the nested
// NavAntiStuck / DNS blocks.
//
// DEVIATIONS: the by-value GetFloat/GetBool overloads take (key, default, flags); the decompiler's
// phantom trailing args are the flags slot (always 0). The decompiler mistyped the GetStr results
// for "moral"/"speedMelee" as section handles and open-coded the string comparison byte-by-byte;
// this reconstruction reads them as strings and maps them via the interned dsSTRID equality helper,
// which is the original intent.
void aiBRAIN_DESC::ReadBehavModifiers(const psSECTION &section)
{
    const psSECTION_INTERFACE &ps = static_cast<const psSECTION_INTERFACE &>(section);
    if (!ps.IsKeyExist("bhvModifiers", 0))
        return;

    psSECTION bmSection = ps.GetSection("bhvModifiers", 0);
    const psSECTION_INTERFACE &bm = static_cast<const psSECTION_INTERFACE &>(bmSection);

    this->radAfraidGrenade = bm.GetFloat("radAfraidGrenade", this->radAfraidGrenade, 0);
    this->towerManStrength = bm.GetFloat("towerManStrength", this->towerManStrength, 0);
    this->isSmtDeathEnabled = bm.GetBool("isSmtDeathEnabled", this->isSmtDeathEnabled, 0) != 0;
    this->smtDeathMaxDist = bm.GetFloat("smtDeathMaxDist", this->smtDeathMaxDist, 0);
    this->avoid3DHOPaths = bm.GetBool("avoid3DHOPaths", this->avoid3DHOPaths, 0) != 0;
    this->enableHOOn3DHOPath = bm.GetBool("enableHOOn3DHOPath", this->enableHOOn3DHOPath, 0) != 0;
    this->disable3DHOMeleePoints = bm.GetBool("disable3DHOMeleePoints", this->disable3DHOMeleePoints, 0) != 0;
    this->force3DHONearEnemy = bm.GetBool("force3DHONearEnemy", this->force3DHONearEnemy, 0) != 0;
    this->force3DHODist = bm.GetFloat("force3DHODist", this->force3DHODist, 0);

    if (bm.IsKeyExist("pathAngleBigTurn", 0))
    {
        float angleRad = bm.GetFloat("pathAngleBigTurn", 180.0f, 0) * 0.017453292f;
        this->dotProdPathBigTurn = (float)cos(angleRad);
    }

    this->alwaysInHO = bm.GetBool("alwaysInHO", this->alwaysInHO, 0) != 0;
    this->distReachMultiplier = bm.GetFloat("distReachMultiplier", this->distReachMultiplier, 0);
    this->distReachLastPoint = bm.GetFloat("distReachLastPoint", this->distReachLastPoint, 0);
    this->timeYieldWay = bm.GetFloat("timeYieldWay", this->timeYieldWay, 0);
    this->distYieldWay = bm.GetFloat("distYieldWay", this->distYieldWay, 0);
    this->mayShootInvisibleEnemy = bm.GetBool("mayShootInvisibleEnemy", this->mayShootInvisibleEnemy, 0) != 0;

    // "moral" archetype string -> enum
    {
        dsTSTRING<char> defEmpty;
        dsTSTRING<char> moralStr = bm.GetStr("moral", defEmpty, 0);
        if (moralStr.pBuffer && moralStr.pBuffer->strLen)
        {
            dsSTRID moralId(moralStr, false);
            if (moralId == "coward")
                this->moral = AI_MORAL_COWARD;
            else if (moralId == "common")
                this->moral = AI_MORAL_COMMON;
            else if (moralId == "bravery")
                this->moral = AI_MORAL_BRAVERY;
        }
    }

    // "speedMelee" -> interned strid
    {
        dsTSTRING<char> defEmpty;
        dsTSTRING<char> speedStr = bm.GetStr("speedMelee", defEmpty, 0);
        if (speedStr.pBuffer && speedStr.pBuffer->strLen)
            this->speedMelee = dsSTRID(speedStr, false);
    }

    if (bm.IsKeyExist("NavAntiStuck", 0))
    {
        psSECTION nasSection = bm.GetSection("NavAntiStuck", 0);
        const psSECTION_INTERFACE &nas = static_cast<const psSECTION_INTERFACE &>(nasSection);

        // Default flag values are recovered from the current weak-edge flag bits.
        unsigned int disabledEdges = this->weakFlagsDisabledEdges;
        unsigned int blockedWP = this->weakFlagsBlockedWP;
        int defPhys = (int)((~(disabledEdges >> 2) | ~(blockedWP >> 1)) & 1);
        int defManual = (int)((~(disabledEdges >> 3) & 1) | ((blockedWP & 4) == 0 ? 1 : 0));

        bool allowPhys = nas.GetBool("allowWalkThruPhys", defPhys, 0) != 0;
        int allowManual = nas.GetBool("allowWalkThruManual", defManual, 0);

        this->weakFlagsDisabledEdges = 0;
        if (!allowPhys)
        {
            this->weakFlagsBlockedWP |= 2;
            this->weakFlagsDisabledEdges = 4;
        }
        if (!allowManual)
        {
            this->weakFlagsBlockedWP |= 4;
            this->weakFlagsDisabledEdges |= 8;
        }
    }

    if (bm.IsKeyExist("DNS", 0))
    {
        psSECTION dnsSection = bm.GetSection("DNS", 0);
        const psSECTION_INTERFACE &di = static_cast<const psSECTION_INTERFACE &>(dnsSection);
        this->dns.nPoints = di.GetInt("nPoints", this->dns.nPoints, 0);
        this->dns.distMin = di.GetFloat("distMin", this->dns.distMin, 0);
        this->dns.distMax = di.GetFloat("distMax", this->dns.distMax, 0);
    }
}
