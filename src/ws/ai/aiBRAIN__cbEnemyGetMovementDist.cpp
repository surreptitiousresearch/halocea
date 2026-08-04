#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::cbEnemyGetMovementDist(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324B958
// ?cbEnemyGetMovementDist@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "EnemyGetMovementDist" getter callback: store implEnemyGetMovementDist's float result into
// `retVal`. `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbEnemyGetMovementDist(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    // impl takes no args; the decompiler's forwarded (argc, argv) are dead callback registers.
    float dist = implEnemyGetMovementDist();
    retVal.StoreValue<float>(dist);
    // the caller ref releases itself at scope end.
}
