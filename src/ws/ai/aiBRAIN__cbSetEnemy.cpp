#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbSetEnemy(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83256680
// ?cbSetEnemy@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetEnemy" setter callback: unbox the enemy-name string out of argv[0] and forward it to the
// virtual implSetEnemy. `caller` is passed by value and its ref self-releases on return.
void aiBRAIN::cbSetEnemy(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    implSetEnemy(name.val);
    // name's string buffer + the caller ref release themselves at scope end.
}
