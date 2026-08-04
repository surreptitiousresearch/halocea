/* hcexHALO_CUSTOM_OBJ_LIST::IsInstManagable @0x823E5578 — decide whether `pInstCD` should be
 * intercepted by this handler instead of the ws-engine's native scenery-instance path:
 *   - breakable surfaces are always managed;
 *   - an instance named "h<sceneryIdx>_<typeId>|h..." (9-char prefix, hex fields, typeId < 12) is
 *     recorded into cdList (sorted, duplicate-ignore) keyed by (typeId<<16 | sceneryIdx+1), and
 *     managed;
 *   - anything else is managed only if its class name is exactly "halo_no_model" (matching
 *     IsSkipCreation's check) — with a diagnostic log for the two "looked like a halo prefix but
 *     wasn't quite right" misses.
 *
 * DEVIATION: the decompiler holds a manually refcounted local dsTSTRING<char> alias of
 * pInstCD->nameInst purely to keep the buffer alive across the several possible early-exit paths.
 * Since nothing in this function can drop pInstCD's own reference while it runs, that bookkeeping
 * is elided here in favor of reading pInstCD->nameInst directly — semantically equivalent, and
 * avoids the decompiler's `local static guard`-lazy-initialized cached string length (verified
 * constant: sizeof("h_______|h")-1 = 9). */

#include "../../headers/hcex/hcexHALO_CUSTOM_OBJ_LIST.h"
#include "../../headers/ws/ds/dsCMP.h"
#include "../../headers/object_type.h"

extern "C" int hcex_is_breakable_surface(scnINST_CREATE_DATA *pInstCD); // boundary
extern "C" int strcmp(const char *a, const char *b);                    // boundary — CRT
extern "C" int sscanf(const char *s, const char *fmt, ...);              // boundary — CRT
void _apLog(const char *fmt, ...);                            // boundary — HCEX log

bool hcexHALO_CUSTOM_OBJ_LIST::IsInstManagable(scnINST_CREATE_DATA *pInstCD)
{
    if ( hcex_is_breakable_surface(pInstCD) )
        return true;

    const char *instName = pInstCD->nameInst.pBuffer->str;
    const int kHaloPrefixLen = 9; // "h?????|h?" — the fixed prefix length this naming scheme uses

    if ( pInstCD->nameInst.pBuffer->strLen == kHaloPrefixLen
         && instName[0] == 'h' && instName[5] == '_' && instName[7] == '|' && instName[8] == 'h' )
    {
        int sceneryIdx = 0;
        unsigned int typeId = 0;
        sscanf(instName, "h%x_%x|h", &sceneryIdx, &typeId);

        if ( sceneryIdx < 0 || typeId >= (unsigned int)number_of_object_types )
        {
            _apLog("~HCEX,Error~ Wrong sceneryid '%x' or typeid '%x' for scene inst '%s'",
                   sceneryIdx, typeId, instName);
            return strcmp(pInstCD->nameClass.pBuffer->str, "halo_no_model") == 0;
        }

        unsigned int key = (typeId << 16) | (unsigned int)(sceneryIdx + 1);
        dsPAIR<unsigned long, scnINST_CREATE_DATA> entry = { key, scnINST_CREATE_DATA(*pInstCD) }; // match cdList element key type
        cdList.InsertSorted(entry, dsCMP(), dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>::INS_DUP_IGNORE);
        return true;
    }

    return strcmp(pInstCD->nameClass.pBuffer->str, "halo_no_model") == 0;
}
