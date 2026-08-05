// entGetClassTplName @ 0x825318C0 — resolve the default name-template of an entity class.
// Empty class name -> empty string. Otherwise look the class up as a brand in the interactive-
// actor family and return its descriptor's default name-template; empty string if not found.
#include "../../headers/ws/ent/entDESC.h"
#include "../../headers/ws/dsc/dscBRAND.h"
#include "../../headers/ws/dsc/dscDESC_FAMILY.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// DEVIATION: the real static type of `iaFamily` is iaFAMILY* (declared in the hcex boundary
// header); iaFAMILY derives — through dscRTTI_FAMILY — from dscDESC_FAMILY. FindBrand is a
// dscDESC_FAMILY method, so the binary calls it on the base slice. Declared here through that
// base to keep this ent TU independent of the hcex/ia headers.
extern dscDESC_FAMILY *iaFamily;

char *entGetClassTplName(const dsTSTRING<char> &nameClass)
{
    // mangled ?entGetClassTplName@@YAPADABV?$dsTSTRING@D@@@Z: arg0 is const dsTSTRING<char>&
    // (the decompiler lowered the reference to a pointer); reference form matches the callers.
    // empty class name -> no template
    if (nameClass.pBuffer->str[0] == '\0')
        return const_cast<char *>(empty_string);

    dscBRAND *brand = iaFamily->FindBrand(nameClass);
    if (brand)
        return reinterpret_cast<entDESC *>(brand->spDesc.pointee)->GetNameTplDefault();
    return const_cast<char *>(empty_string);
}
