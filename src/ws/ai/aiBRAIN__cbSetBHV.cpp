#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbSetBHV(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83256ED0
// ?cbSetBHV@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetBHV" setter callback: unbox the behaviour-name string out of argv[0] and forward it to the
// virtual implSetBHV. `caller` is passed by value and its ref self-releases on return.
void aiBRAIN::cbSetBHV(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    implSetBHV(name.val);
}
