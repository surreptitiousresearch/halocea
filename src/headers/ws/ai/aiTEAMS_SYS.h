#pragma once
#include "../ds/dsSTRID.h"
#include "../m3d/m3dV.h"
// ws-engine ai08: teams subsystem. As embedded in aiPLANNER it is a 4-byte placeholder class
// (DB member i_am_not_empty_class); its team queries are thin helpers over the gs team registry
// (gsTEAMS). Static id helpers plus per-instance attitude queries against propGAME_INFO_TEAM.

struct propGAME_INFO_TEAM; // prop/propGAME_INFO_TEAM.h
struct m3dCOLOR;           // m3d/m3dCOLOR.h

struct aiTEAMS_SYS {
    int i_am_not_empty_class; // 0x00 (DB-verified aiPLANNER.teams filler)

    // Return the interned team id of the local player(s) (sret out-param). Lazily interns
    // "PLAYERS" on first call (function-local static). 0x83172EA0.
    static dsSTRID *PlayersTeamID(dsSTRID *result);

    // 0x83172E70 (?NeutralTeamID@aiTEAMS_SYS@@SA?AVdsSTRID@@XZ) — the interned gs neutral team id
    // (sret out-param). Thin forwarder over gsTEAMS::NeutralTeamID.
    static dsSTRID *NeutralTeamID(dsSTRID *result);

    // 0x83172F20 — true when teamID is the players' team.
    static bool IsPlayersTeam(const dsSTRID *teamID);
    // 0x83172F68 — true when teamID is the gs neutral team.
    static bool IsNeutralTeam(const dsSTRID *teamID);
    // 0x83172FB0 — true when teamID is either the players' or the neutral team.
    static bool IsPredefinedTeam(const dsSTRID *teamID);

    // 0x83173050 — teamIDFrom is an enemy of teamIDTo.
    bool IsEnemy(const dsSTRID &teamIDFrom, const dsSTRID &teamIDTo) const;
    // 0x831730A0 — same, over propGAME_INFO_TEAM handles.
    bool IsEnemy(const propGAME_INFO_TEAM &teamFrom, const propGAME_INFO_TEAM &teamTo) const;
    // 0x831730E8 — teamFrom is an enemy of the players' team.
    bool IsEnemyToPlayer(const propGAME_INFO_TEAM &teamFrom) const;
    // 0x83173140 — teamFrom is neutral toward teamTo.
    bool IsNeutral(const dsSTRID &teamFrom, const dsSTRID &teamTo) const;
    // 0x831734D8 — same, over propGAME_INFO_TEAM handles. REVERSED: aiTEAMS_SYS__IsNeutral.cpp.
    bool IsNeutral(const propGAME_INFO_TEAM &teamFrom, const propGAME_INFO_TEAM &teamTo) const;

    // 0x83173520 — count active AI units within `radius` of `pos` whose team is neutral toward
    // `teamMy`. REVERSED: aiTEAMS_SYS__CountMyNeutralsNearPoint.cpp.
    int CountMyNeutralsNearPoint(const propGAME_INFO_TEAM &teamMy, const m3dV &pos, float radius) const;

    // 0x83173AC8 — install the default teams property-script ("default_teams.ps") into the gs
    // registry. REVERSED: aiTEAMS_SYS__ProcessINIT.cpp.
    void ProcessINIT();

    // 0x831731F0 — radar blip colour for team (sret m3dCOLOR): red when enemy of the player,
    // green otherwise; alpha 255. Returns `this`.
    m3dCOLOR *GetRadarMarkColor(m3dCOLOR *result, const propGAME_INFO_TEAM &team) const;

    // 0x83173298 — a chaser should notice team's death (team is an enemy of the player).
    bool ShouldChaserNoticeDeath(const propGAME_INFO_TEAM &team) const;
    // 0x831732F0 — team is accepted by a chaser (team is an enemy of the player).
    bool IsAcceptedByChaser(const propGAME_INFO_TEAM &team) const;
    // 0x83173358 — debug colour mask: 0xFFFFFFFF when enemy of the player, 0xFFDFDFDF otherwise.
    unsigned int GetDbgColorMask(const propGAME_INFO_TEAM &team) const;
    // 0x83173350 (?DropAll@aiTEAMS_SYS@@QAAXXZ) — drop all team state. boundary.
    void DropAll();
    // 0x83173348 (?ProcessINIT_LEVEL@aiTEAMS_SYS@@QAAXXZ) — per-level init. boundary.
    void ProcessINIT_LEVEL();
};
