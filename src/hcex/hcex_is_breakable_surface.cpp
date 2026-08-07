// hcex_is_breakable_surface @ 0x823B2910
//
// Classify a scene instance's create-data as a breakable surface and, if so, register a bridge slot
// for it. A slot qualifies when its entity-class name is exactly "breakable_glass" and its instance
// name both starts with "brk_" and ends with "|h". For a match, a hcexBREAKABLE_SURF is built from
// the create-data, its (idx,bsp) key is parsed out of the instance name ("brk_%x_%d|h"), and it is
// inserted into the sorted hcexBreakableSurfaces vector (duplicates ignored). Returns whether the
// create-data was a breakable surface.
//
// DEVIATION: the disassembly inlines the "breakable_glass" comparison as a hand-rolled byte loop;
// reproduced here as the equivalent strcmp. The sscanf targets mirror the disassembly exactly —
// the hex field is parsed into `idx` (0x04) and the decimal field into `bsp` (0x00).

#include "../headers/hcex/hcex_surf_vector.h" // hcexBREAKABLE_SURF, scnINST_CREATE_DATA, dsVECTOR
#include "../headers/ws/ds/dsTSTRING.h"

extern "C" int   strcmp(const char *a, const char *b);       // boundary — CRT
extern "C" int   sscanf(const char *s, const char *fmt, ...); // boundary — CRT
extern "C" void *memset(void *dst, int val, unsigned int n);  // boundary — CRT

extern hcex_surf_vector hcexBreakableSurfaces;

typedef int dsCMP; // stateless comparator selector for InsertSorted (DB dsCMP)

bool hcex_is_breakable_surface(scnINST_CREATE_DATA *pInstCD)
{
    if (strcmp(pInstCD->nameClass.pBuffer->str, "breakable_glass") != 0)
        return false;

    if (!pInstCD->nameInst.IsStartWith("brk_", 0, 0) || !pInstCD->nameInst.IsEndWith("|h", 0))
        return false;

    hcexBREAKABLE_SURF surf;              // ctor default-constructs only the embedded cd
    memset(&surf, 0, 12);                 // zero the bsp/idx/pInst key that the ctor leaves alone
    surf.cd = *pInstCD;                   // scnINST_CREATE_DATA::operator=
    sscanf(pInstCD->nameInst.pBuffer->str, "brk_%x_%d|h", &surf.idx, &surf.bsp);

    dsCMP cmp;
    hcexBreakableSurfaces.InsertSorted(surf, cmp, dsVECTOR<hcexBREAKABLE_SURF, 8>::INS_DUP_IGNORE);
    return true;
}
