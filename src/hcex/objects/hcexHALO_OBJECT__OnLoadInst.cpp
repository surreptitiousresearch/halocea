/* hcexHALO_OBJECT::OnLoadInst @0x823D6BA0 (overrides entMANAGER_HANDLER's load-instance hook) —
 * after the base engine's pre-init handling, mark the instance initialized (state |= 2) and walk
 * its "permutations" section: for each key (a region name), find the matching object on the
 * instance by case-insensitive name; if found, resolve the key's string value (a substitute
 * region/object name) and find it under that region (case-insensitive); when both resolve, toggle
 * process visibility (region off, substitute on); log a diagnostic when either lookup misses.
 *
 * CAVEAT: the decompiler's local-variable allocation clearly failed across this function — it
 * declares a `psITERATOR v31[3]` array and reuses overlapping byte ranges of that array (and of
 * several `psSECTION`/`dsTSTRING` BYREF locals) for unrelated values, none of which correspond to
 * real distinct objects. This reconstruction instead follows the disassembly's call sequence and
 * (idasql-verified) per-call argument names directly: a single real psSECTION (permutations), a
 * single real psITERATOR (the outer loop cursor), and one psSECTION/dsSTRID/dsTSTRING temporary
 * per resolved value, matching each call site's actual `this`/arg registers rather than the
 * decompiler's fabricated variable list. */

#include "../../headers/hcex/hcexHALO_OBJECT.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/gs/gsLVL_SYSTEM.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../../headers/ws/ps/psITERATOR.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/obj/objFILTER_NAME_NOCASE.h"
#include "../../headers/ws/obj/objOBJ.h"

extern gsLVL_SYSTEM *gsSysLevel; // boundary global — the active level system
void _apLog(const char *fmt, ...); // boundary — HCEX log
extern objOBJ *objFind(objOBJ *pObj, objFILTER *pObjFilter); // boundary — depth-first predicate search

void hcexHALO_OBJECT::OnLoadInst(animINST *pInst)
{
    gsSysLevel->HandleAnimInstBeforeMSG_INIT(pInst);
    pInst->state |= 2;

    psSECTION ps = pInst->GetPS();
    psSECTION permutations =
        static_cast<psSECTION_INTERFACE &>(ps).GetSection("permutations", 0);

    if ( static_cast<psSECTION_INTERFACE &>(permutations).IsNull() ) // DB: psSECTION_INTERFACE::IsNull (handle absent)
        return;

    psITERATOR iter(permutations, PS_TYPEID_STRING, 0);
    while ( !iter.IsDone() )
    {
        psSECTION keySection;
        iter.GetKeySection(&keySection);
        dsSTRID regionKeyName =
            static_cast<psSECTION_INTERFACE &>(keySection).GetKeyName(iter.GetKeyIdx());

        objFILTER_NAME_NOCASE regionFilter(regionKeyName.CStr());
        objOBJ *regionObj = objFind(pInst->pObj, &regionFilter);

        if ( !regionObj )
        {
            _apLog("~HCEX,Error~ Can't find region object '%s' in '%s' tpl for '%s' inst",
                   regionKeyName.CStr(), pInst->pTpl->name, pInst->GetName());
        }
        else
        {
            dsTSTRING<char> emptyDef = {};
            dsTSTRING<char> substituteName = {};
            iter.GetStr(&substituteName, emptyDef); // sret: substituteName <- this key's string value

            objFILTER_NAME_NOCASE substituteFilter(substituteName.CStr());
            objOBJ *substituteObj = objFind(regionObj, &substituteFilter);

            if ( substituteObj )
            {
                regionObj->SetStateProcYes(8, nullptr);
                substituteObj->SetStateProcNo(8, nullptr);
            }
            else
            {
                _apLog("~HCEX,Error~ Can't find object '%s' in '%s' tpl for region '%s' of inst '%s'",
                       substituteName.CStr(), pInst->pTpl->name, regionKeyName.CStr(), pInst->GetName());
            }
        }

        iter.Next();
    }
}
