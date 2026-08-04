#include "../headers/hcex/hcexHALO_OBJECT.h"
#include "../headers/animINST.h"
#include "../headers/animTPL.h"
#include "../headers/ws/gs/gsLVL_SYSTEM.h"
#include "../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../headers/ws/ps/psITERATOR.h"
#include "../headers/ws/obj/objOBJ.h"
#include "../headers/ws/obj/objFILTER_NAME_NOCASE.h"
#include "../headers/ws/obj/obj_free_functions.h"

// ws-engine level system singleton (boundary global)
extern gsLVL_SYSTEM *gsSysLevel;
// ws-engine logging boundary
extern void _apLog(const char *format, ...);

// 0x823D6B88
// Region-substitution hook for Halo-model entities: for each key in the animINST's "permutations"
// section, find the named region object under the inst, then find the named permutation object
// beneath it, and toggle their state-proc-8 (show region root / hide the chosen permutation).
void hcexHALO_OBJECT::OnLoadInst(animINST *pInst)
{
    gsSysLevel->HandleAnimInstBeforeMSG_INIT(pInst);
    pInst->state |= 2u;

    // psSECTION and psSECTION_INTERFACE are both 4-byte handle wrappers over the same section body;
    // the binary calls the query methods by viewing a psSECTION as a psSECTION_INTERFACE.
    psSECTION ps = pInst->GetPS();
    psSECTION permutations = reinterpret_cast<psSECTION_INTERFACE *>(&ps)->GetSection("permutations", 0);
    if ( reinterpret_cast<psSECTION_INTERFACE *>(&permutations)->IsEmpty() )
        return;

    for ( psITERATOR it(permutations, PS_TYPEID_STRING, 0); !it.IsDone(); it.Next() )
    {
        psSECTION keySection;
        it.GetKeySection(&keySection);
        dsSTRID regionKeyName = reinterpret_cast<psSECTION_INTERFACE *>(&keySection)->GetKeyName(it.GetKeyIdx());
        const char *regionName = regionKeyName.CStr();

        objFILTER_NAME_NOCASE regionFilter(regionName);
        objOBJ *regionObj = objFind(pInst->pObj, &regionFilter);
        if ( regionObj )
        {
            dsTSTRING<char> permStr;
            dsTSTRING<char> empty;
            const char *permName = it.GetStr(&permStr, empty)->pBuffer->str;
            objFILTER_NAME_NOCASE permFilter(permName);
            objOBJ *permObj = objFind(regionObj, &permFilter);
            if ( permObj )
            {
                regionObj->SetStateProcYes(8, nullptr);
                permObj->SetStateProcNo(8, nullptr);
            }
            else
            {
                _apLog("~HCEX,Error~ Can't find object '%s' in '%s' tpl for region '%s' of inst '%s'",
                       permName, pInst->pTpl->name, regionName, pInst->GetName());
            }
        }
        else
        {
            _apLog("~HCEX,Error~ Can't find region object '%s' in '%s' tpl for '%s' inst",
                   regionName, pInst->pTpl->name, pInst->GetName());
        }
    }
}
