#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h" // DSD_CONV_RETRIEVE
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR_PARAM_LIST.h"

// aiBRAIN::cbQOT_Put @ 0x8325BC18
// ?cbQOT_Put@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback for "QOT_Put(p : params)": retrieve the param-list reference from argv[0] into a fresh
// REF_TYPE (default ctor allocates the shared REF_TYPE_DATA, operator new(0xC) at 0x8325BC40), then
// forward it by value to implQOT_Put. Structurally identical to cbTestParam2AI.
void aiBRAIN::cbQOT_Put(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> value;
    argv->GetValue<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> >(value, DSD_CONV_RETRIEVE);
    implQOT_Put(value);
}
