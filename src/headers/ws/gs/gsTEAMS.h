#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
// ws-engine gs: the global team registry singleton. Only the surface consumed by
// aiTEAMS_SYS is modelled here; the map/attitude storage is an extern boundary
// (descend in a later wave). DB: ?Instance@gsTEAMS@@SAAAV1@XZ @0x82D31220,
// ?GetAttitude@gsTEAMS@@QBA?AW4ATTITUDE@1@ABVdsSTRID@@0@Z @0x828B0AF8,
// ?NeutralTeamID@gsTEAMS@@SA?AVdsSTRID@@XZ @0x828AA790.

struct gsTEAMS {
    // Attitude between two teams. DB types_enum_values (gsTEAMS::ATTITUDE):
    // ATT_NEUTRAL=0, ATT_ENEMY=1, ATT_FRIEND=2.
    enum ATTITUDE {
        ATT_NEUTRAL = 0,
        ATT_ENEMY   = 1,
        ATT_FRIEND  = 2,
    };

    // Process-wide singleton accessor. boundary.
    static gsTEAMS &Instance();

    // Relationship of teamFrom toward teamTo. boundary.
    ATTITUDE GetAttitude(const dsSTRID &teamFrom, const dsSTRID &teamTo) const;

    // ?IsTeamRegistered@gsTEAMS@@QBA_NABVdsSTRID@@@Z @0x828AFCF0 — is `team` a registered team. boundary.
    bool IsTeamRegistered(const dsSTRID &team) const;

    // ?SetAttitude@gsTEAMS@@QAA_NABVdsSTRID@@0W4ATTITUDE@1@@Z @0x828B3D20 — set attitude of
    // teamFrom toward teamTo; returns true on success. boundary.
    bool SetAttitude(const dsSTRID &teamFrom, const dsSTRID &teamTo, ATTITUDE att);

    // ?SetDefaultPS@gsTEAMS@@QAAXABV?$dsTSTRING@D@@@Z @0x828ADCF0 — set the default teams
    // property-script file. boundary.
    void SetDefaultPS(const dsTSTRING<char> &psName);

    // Interned id of the reserved neutral team (sret out-param). boundary.
    static dsSTRID *NeutralTeamID(dsSTRID *result);
};
