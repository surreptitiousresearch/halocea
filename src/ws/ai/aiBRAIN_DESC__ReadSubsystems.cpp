#include "../../headers/ws/ai/aiBRAIN_DESC_parse_boundary.h"

// aiBRAIN_DESC::ReadSubsystems(const psSECTION&) @ 0x83188618
// Read the "subsystems" sub-section, resolving each named AI subsystem to one of its acceptable
// subsystem-set variants (defaulting to the field's current value when absent/invalid).
void aiBRAIN_DESC::ReadSubsystems(const psSECTION &psReadS)
{
    const psSECTION_INTERFACE &psRead = static_cast<const psSECTION_INTERFACE &>(psReadS);
    if (!psRead.IsKeyExist("subsystems", 0))
        return;

    psSECTION sub = psRead.GetSection("subsystems", 0);
    psSECTION_INTERFACE *subIn = static_cast<psSECTION_INTERFACE *>(&sub);

    // Two validation sets used by the various subsystems.
    dsVECTOR<dsSTRID, 8> allowNullFull;       // {NULL, FULL}
    dsVECTOR<dsSTRID, 8> allowNullIdleBattle; // {NULL, IDLE, BATTLE}
    allowNullFull.PushBack(ssTYPE_NULL);
    allowNullFull.PushBack(ssTYPE_FULL);
    allowNullIdleBattle.PushBack(ssTYPE_NULL);
    allowNullIdleBattle.PushBack(ssTYPE_IDLE);
    allowNullIdleBattle.PushBack(ssTYPE_BATTLE);

    ssTypeBody   = ReadAndCheckSystemType(subIn, "body",     ssTypeBody,   &allowNullFull);
    ssTypeAim    = ReadAndCheckSystemType(subIn, "aim",      ssTypeAim,    &allowNullIdleBattle);
    ssTypeAprog  = ReadAndCheckSystemType(subIn, "approach", ssTypeAprog,  &allowNullFull);
    ssTypeBest   = ReadAndCheckSystemType(subIn, "best",     ssTypeBest,   &allowNullFull);
    ssTypeEnemy  = ReadAndCheckSystemType(subIn, "enemy",    ssTypeEnemy,  &allowNullFull);
    ssTypeGoal   = ReadAndCheckSystemType(subIn, "goal",     ssTypeGoal,   &allowNullFull);
    ssTypePerc   = ReadAndCheckSystemType(subIn, "perc",     ssTypePerc,   &allowNullFull);
    ssTypeVision = ReadAndCheckSystemType(subIn, "vision",   ssTypeVision, &allowNullIdleBattle);
    ssTypeSelene = ReadAndCheckSystemType(subIn, "selectEn", ssTypeSelene, &allowNullFull);
    ssTypeIsa    = ReadAndCheckSystemType(subIn, "isa",      ssTypeIsa,    &allowNullFull);
    ssTypeSnd    = ReadAndCheckSystemType(subIn, "snd",      ssTypeSnd,    &allowNullFull);
    ssTypeTask   = ReadAndCheckSystemType(subIn, "task",     ssTypeTask,   &allowNullFull);
}
