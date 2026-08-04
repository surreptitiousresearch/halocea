#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/ds/dsCONST_ARRAY.h"
#include "../../headers/ws/ds/dsAFFIX_STRING.h"
#include "../../headers/entCREATE_DATA.h"
#include "../../headers/ws/ps/psSECTION.h"
#include <string.h>

// aiDOM_SPAWN::InitCreateData @ 0x83264B80
// ?InitCreateData@aiDOM_SPAWN@@MAAXAAVentCREATE_DATA@@VpsSECTION@@V?$dsTSTRING@D@@@Z (virtual)
// Fill in the create-data for one spawned instance: unique name "<class>__<domain>_spwn_<idx>",
// an optional affix drawn from the desc's affix list (desc[31]), and a small vertical drop so the
// spawned unit sits on the floor. `ps` and `nameCls` are passed by value.
void aiDOM_SPAWN::InitCreateData(entCREATE_DATA &cd, psSECTION ps, dsTSTRING<char> nameCls)
{
    // Seed cd from the gs base (domain / ps).
    gsDOM_SPAWN_InitCreateData(this, &cd, &ps, &nameCls);

    // cd.name = "<class>__<domainName>_spwn_<idx>"
    int idx = ent_idx++;
    dsTSTRING<char> myName = this->GetName();
    dsTSTRING<char> full;
    dsSPrintf(&full, "%s__%s_spwn_%d", nameCls.pBuffer->str, myName.pBuffer->str, idx);
    strncpy(cd.name, full.pBuffer->str, 0x3F);

    if (this->spDesc.pointee[31].__vftable) // affix list present
    {
        int affixIdx = this->rxAffix.Get();
        const dsTSTRING<char> &affix =
            (*(dsCONST_ARRAY<dsTSTRING<char>, 20> *)&this->spDesc.pointee[31])[affixIdx];
        dsAFFIX_STRING picked(affix);
        cd.affixes = picked;
        cd.matrInst.elements[13] = cd.matrInst.elements[13] - dipGEN_SPAWN::GetShiftOverFloor();
    }
    else
    {
        dsAFFIX_STRING empty{dsTSTRING<char>()}; // UnsafeInitEmpty in the original; braces avoid the most-vexing parse
        cd.affixes = empty;
    }
}
