#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"

// aiReadStridFlagsFromPS @ 0x832D38A0
// ?aiReadStridFlagsFromPS@@YAXAAUdsSTRID_FLAGS@@PBDABVpsSECTION@@@Z
// Read a strid-flag set stored under `key` into `out`. boundary.
extern void aiReadStridFlagsFromPS(dsSTRID_FLAGS &out, const char *key, const psSECTION &section);

// aiBRAIN_DESC::ReadHealthDeath @ 0x83185E10
// ?ReadHealthDeath@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z
//
// Read the injury / enemy-damage-reaction parameters, clamp injTimeDisable to be at least
// injTimeLockEnemy, then read the damage-immunity filter flags.
// (The decompiler's trailing `a3` parameter is a phantom GetFloat flags slot, not a real argument.)
void aiBRAIN_DESC::ReadHealthDeath(const psSECTION &section)
{
    const psSECTION_INTERFACE &ps = static_cast<const psSECTION_INTERFACE &>(section);

    this->minEnemyDamaged     = ps.GetFloat("minEnemyDamaged", this->minEnemyDamaged, 0);
    this->speedEnemyDamagedDec = ps.GetFloat("speedEnemyDamagedDec", this->speedEnemyDamagedDec, 0);
    this->injDmgToChangeEnemy = ps.GetFloat("injDmgToChangeEnemy", this->injDmgToChangeEnemy, 0);
    this->injTimeLockEnemy    = ps.GetFloat("injTimeLockEnemy", this->injTimeLockEnemy, 0);
    this->injTimeDisable      = ps.GetFloat("injTimeDisable", this->injTimeDisable, 0);
    this->injWndDecreaseSpeed = ps.GetFloat("injWndDecreaseSpeed", this->injWndDecreaseSpeed, 0);

    if (this->injTimeLockEnemy > this->injTimeDisable)
        this->injTimeDisable = this->injTimeLockEnemy;

    aiReadStridFlagsFromPS(this->sfilterImmune, "sfilterImmune", section);
}
