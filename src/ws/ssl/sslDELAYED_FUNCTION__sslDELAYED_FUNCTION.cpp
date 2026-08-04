#include "sslDELAYED_FUNCTION.h"

// sslDELAYED_FUNCTION::sslDELAYED_FUNCTION() @ 0x82AF0970
// Construct an empty/idle record: default-construct self/caller (boundary sslOBJ_REF default
// ctor), empty the params vector (tagging its allocation-site cookie with the original source
// location, ssl_stack.cpp:838 -- reproduced verbatim from the binary's embedded string/line),
// default-construct group (boundary dsSTRID default ctor, null id), null out syncContext's
// runtime-type pointer (matches the dsDATA {type=nullptr, uninitialised storage} pattern seen
// elsewhere, e.g. sslSTACK_DATA::ConvertToDsData), then reset the scalar bookkeeping fields.
sslDELAYED_FUNCTION::sslDELAYED_FUNCTION()
    : self(), caller()
{
    this->params.pData = nullptr;
    this->params.nElem = 0;
    this->params.allocated = 0;
    this->params.__cl.file = "D:\\Projects\\code\\common\\src.sys\\ssl\\ssl_stack.cpp";
    this->params.__cl.line = 838;

    this->group = dsSTRID();

    this->syncContext.type = nullptr;

    this->pauseLocks = 0;
    this->funcIdx = -1;
    this->timeLeft = 0.0f;
}
