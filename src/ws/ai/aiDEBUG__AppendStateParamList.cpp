#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR_PARAM_LIST.h"
#include "../../headers/ws/ds/dsPARAM.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include <string.h>

// aiDEBUG::AppendStateParamList @ 0x83196BF8
//   (?AppendStateParamList@aiDEBUG@@SAXABV?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@AAV?$dsTSTRING@D@@@Z)
//
// Static: append each parameter of `p`'s owned list to `line` in the form
//   "  <name> = <debug-value>" (or "  <name> (empty) "), one per line (CR/LF + two-space indent).
//
// The decompiler models the per-parameter value string via a reused stack dsDATA slot; it is the
// sret return of dsDATA::GetDbgString (a dsTSTRING<char>). Reconstructed here as that value string.
void aiDEBUG::AppendStateParamList(const ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> &p,
                                   dsTSTRING<char> &line)
{
    line.Insert(line.pBuffer->strLen, "  ", -1);

    dsVECTOR_PARAM_LIST *params = p.m_pData->m_pT;
    int length = params->length;
    if (length <= 0)
        return;

    dsPARAM *param = params->list;
    for (int i = 0; i < length; ++i, ++param)
    {
        // parameter name
        line.Insert(line.pBuffer->strLen, param->id.CStr(), -1);

        // parameter value rendered as a debug string
        dsTSTRING<char> value = param->data.GetDbgString();
        if (value.pBuffer->strLen != 0)
        {
            // Binary inlines the protected grow-and-shift Insert(pos,len)+memcpy of (" = " + value);
            // reconstructed via the public Insert(pos, const dsTSTRING&) overload (ABI-equivalent).
            dsTSTRING<char> assigned = " = " + value;
            line.Insert(line.pBuffer->strLen, assigned);
        }
        else
        {
            line.Insert(line.pBuffer->strLen, " (empty) ", -1);
        }

        if (i < length - 1)
            line.Insert(line.pBuffer->strLen, "\r\n  ", -1);
    }
}
