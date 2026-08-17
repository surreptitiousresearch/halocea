/* ?IsInstManagable@hcexHALO_CUSTOM_OBJ_LIST@@UAA_NPAUscnINST_CREATE_DATA@@@Z @0x823E5560 */
#include "../headers/hcex/hcexHALO_CUSTOM_OBJ_LIST.h"
#include "../headers/ws/ds/dsCMP.h"
#include "../headers/object_type.h"
#include <cstdio> // sscanf (CRT boundary)

// hcex free-function boundary (Blam-side breakable-surface classification)
extern bool hcex_is_breakable_surface(scnINST_CREATE_DATA *pInstCD);
// ws-engine logging boundary
extern void _apLog(const char *format, ...);

// 0x823E5560
// Decide whether a scene-instance creation should be intercepted by this handler:
//   - true for breakable surfaces;
//   - for "h<sceneryIdx>_<typeId>|h..." named halo-model instances (name length 9), record the
//     create data into cdList (sorted, duplicate-ignore) and return true;
//   - otherwise return whether the instance's class name is "halo_no_model" (== IsSkipCreation).
bool hcexHALO_CUSTOM_OBJ_LIST::IsInstManagable(scnINST_CREATE_DATA *pInstCD)
{
    if ( hcex_is_breakable_surface(pInstCD) )
        return true;

    // Copy the instance name (adopts the buffer, bumping its refcount for the lifetime of nameInst).
    dsTSTRING<char> nameInst = pInstCD->nameInst;

    static const int nameLen = 9; // strlen of "h####_#|h"

    if ( nameLen == nameInst.pBuffer->strLen
        && nameInst[0] == 'h'
        && nameInst[5] == '_'
        && nameInst[7] == '|'
        && nameInst[8] == 'h' )
    {
        int sceneryIdx = 0;
        int typeId = 0;
        sscanf(nameInst.pBuffer->str, "h%x_%x|h", &sceneryIdx, &typeId);

        if ( sceneryIdx < 0 || (unsigned)typeId >= number_of_object_types )
        {
            _apLog("~HCEX,Error~ Wrong sceneryid '%x' or typeid '%x' for scene inst '%s'",
                   sceneryIdx, typeId, nameInst.pBuffer->str);
            // original inlines the "halo_no_model" strcmp (== IsSkipCreation)
            return IsSkipCreation(pInstCD);
        }

        dsPAIR<unsigned long, scnINST_CREATE_DATA> entry;
        entry.key = (typeId << 16) | (sceneryIdx + 1);
        entry.val = *pInstCD;
        dsCMP cmp;
        cdList.InsertSorted<dsCMP>(entry, cmp,
                                   dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>::INS_DUP_IGNORE);
        return true;
    }

    // original inlines the "halo_no_model" strcmp (== IsSkipCreation)
    return IsSkipCreation(pInstCD);
}
