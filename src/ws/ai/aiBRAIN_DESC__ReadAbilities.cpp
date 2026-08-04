#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ps/psSECTION.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"

// aiBRAIN_DESC::ReadAbilities @ 0x83185788
// Read the optional "abilities" subsection, overriding each ability toggle in `abil` (defaulting to
// its current value when the key is absent). psRead is a psSECTION handle; the section-query API
// lives on psSECTION_INTERFACE, which shares psSECTION's single-pointer layout, so both `psRead`
// and the extracted sub-section are viewed through that interface.
void aiBRAIN_DESC::ReadAbilities(const psSECTION &psRead)
{
    const psSECTION_INTERFACE &root = reinterpret_cast<const psSECTION_INTERFACE &>(psRead);
    if (!root.IsKeyExist("abilities", 0))
        return;

    psSECTION sub = root.GetSection("abilities", 0);
    const psSECTION_INTERFACE &s = reinterpret_cast<const psSECTION_INTERFACE &>(sub);

    this->abil.HO                      = s.GetBool("useHO", this->abil.HO, 0) != 0;
    this->abil.NS                      = s.GetBool("useNS", this->abil.NS, 0) != 0;
    this->abil.relocalize              = s.GetBool("abilRelocalize", this->abil.relocalize, 0) != 0;
    this->abil.jumpOverHO              = s.GetBool("useJumpOverHO", this->abil.jumpOverHO, 0) != 0;
    this->abil.rollBetweenHO           = s.GetBool("useRollBetweenHO", this->abil.rollBetweenHO, 0) != 0;
    this->abil.smartObjects            = s.GetBool("useSmartObjects", this->abil.smartObjects, 0) != 0;
    this->abil.minizoom                = s.GetBool("useMinizoom", this->abil.minizoom, 0) != 0;
    this->abil.blindShoot              = s.GetBool("useBlindShoot", this->abil.blindShoot, 0) != 0;
    this->abil.floorPlace              = s.GetBool("floorPlace", this->abil.floorPlace, 0) != 0;
    this->abil.visionReal              = s.GetBool("visionReal", this->abil.visionReal, 0) != 0;
    this->abil.reload                  = s.GetBool("abilReload", this->abil.reload, 0) != 0;
    this->abil.is3DMove                = s.GetBool("abil3DMove", this->abil.is3DMove, 0) != 0;
    this->abil.melee                   = s.GetBool("abilMelee", this->abil.melee, 0) != 0;
    this->abil.miniGameMelee           = s.GetBool("abilMeleeMinigame", this->abil.miniGameMelee, 0) != 0;
    this->abil.optimizable             = s.GetBool("abilOptimizable", this->abil.optimizable, 0) != 0;
    this->abil.boss                    = s.GetBool("abilBoss", this->abil.boss, 0) != 0;
    this->abil.reactToReticle          = s.GetBool("abilReactToReticle", this->abil.reactToReticle, 0) != 0;
    this->abil.aimAtNeverSeenEnemy     = s.GetBool("abilAimAtNeverSeenEnemy", this->abil.aimAtNeverSeenEnemy, 0) != 0;
    this->abil.shootAtIdleEnemy        = s.GetBool("abilShootAtIdleEnemy", this->abil.shootAtIdleEnemy, 0) != 0;
    this->abil.throwGrenadeAtOpenEnemy = s.GetBool("abilThrowGrenadeAtOpenEnemy", this->abil.throwGrenadeAtOpenEnemy, 0) != 0;
    this->abil.grenadeAvoidance        = s.GetBool("abilGrenadeAvoidance", this->abil.grenadeAvoidance, 0) != 0;
    this->abil.physObjAvoidance        = s.GetBool("abilPhysObjAvoidance", this->abil.physObjAvoidance, 0) != 0;
    this->abil.acceptFriendlyFire      = s.GetBool("abilAcceptFriendlyFire", this->abil.acceptFriendlyFire, 0) != 0;
    this->abil.changeEnemyDuringStrike = s.GetBool("abilChangeEnemyDuringStrike", this->abil.changeEnemyDuringStrike, 0) != 0;
    this->abil.checkVisForBlindShoot   = s.GetBool("CheckVisForBlindShoot", this->abil.checkVisForBlindShoot, 0) != 0;
    this->abil.tinyDamageFromPlayer    = s.GetBool("abilIsTinyDamageFromPlayer", this->abil.tinyDamageFromPlayer, 0) != 0;
}
