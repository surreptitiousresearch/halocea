#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h" // DSD_CONV_RETRIEVE
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR_PARAM_LIST.h"

// aiBRAIN::cbTestParam2AI @ 0x8325BB78
// ?cbTestParam2AI@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback for "TestParam2AI(p : params)": retrieve the param-list reference from argv[0] into a
// fresh REF_TYPE (its default ctor allocates the shared REF_TYPE_DATA seen as operator new(0xC) at
// 0x8325BBA0), then forward it by value to implTestParam2AI. The by-value pass is the refcount-bumped
// copy the disassembly makes just before the call; both the local and the copy release on return, as
// does the incoming `caller` sslOBJ_REF.
void aiBRAIN::cbTestParam2AI(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> value;
    argv->GetValue<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> >(value, DSD_CONV_RETRIEVE);
    implTestParam2AI(value);
}
