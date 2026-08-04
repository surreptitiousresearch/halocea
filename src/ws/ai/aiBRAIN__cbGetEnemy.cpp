#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// aiBRAIN::cbGetEnemy @ 0x83248BA0
// ?cbGetEnemy@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: return the brain's current enemy actor's SSL object reference (an empty ref when
// there is no enemy) into `retVal`.
void aiBRAIN::cbGetEnemy(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;

    iaIACTOR *enemy = implGetEnemy();

    sslOBJ_REF ref; // empty ref when the brain has no enemy
    if (enemy)
        ref = enemy->sslObject;

    dsDATA asData = ref; // sslOBJ_REF::operator dsDATA()
    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue(asData);
    retVal.StoreValue(tmp);
}
