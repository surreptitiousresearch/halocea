#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbAttachBodyToSmtObj(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83258568
// ?cbAttachBodyToSmtObj@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "AttachBodyToSmtObj" callback: unbox (object, anchor) out of argv and forward to the virtual
// implAttachBodyToSmtObj; store its bool result into `retVal`. `caller` self-releases on return.
void aiBRAIN::cbAttachBodyToSmtObj(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > object(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > anchor(argv[1], nullptr);

    bool result = implAttachBodyToSmtObj(object.val, anchor.val);
    retVal.StoreValue<bool>(result);
}
