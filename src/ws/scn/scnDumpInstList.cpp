// scnDumpInstList @ 0x8253D1A0 — dump every live animINST in `pScn` to the log file `nameFile`
// (and to the debug output), one CSV row of "nameClass; nameTpl; nameInst" per instance.
#include "../../headers/ws/scn/scnSCENE.h"
#include "../../headers/animINST.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// boundary — variadic logging helpers (ap_log.cpp / os debug output).
extern void _apForceLog(const char *fileName, const char *fmt, ...);
extern "C" void osOutputDebugString(const char *fmt, ...);

void scnDumpInstList(scnSCENE *pScn, const dsTSTRING<char> *nameFile)
{
    _apForceLog(nameFile->pBuffer->str, "nameClass; nameTpl; nameInst\n");
    osOutputDebugString("Inst list:\n");

    for (animINST *inst = pScn->pFirstInst; inst; inst = inst->next)
    {
        const char *name      = inst->name      ? inst->name      : empty_string;
        const char *nameTpl   = inst->nameTpl   ? inst->nameTpl   : empty_string;
        const char *nameClass = inst->nameClass ? inst->nameClass : empty_string;

        osOutputDebugString("%s; %s; %s\n", nameClass, nameTpl, name);
        _apForceLog(nameFile->pBuffer->str, "%s; %s; %s\n", nameClass, nameTpl, name);
    }

    // null format -> flush and close the log file (apForceLogV string==null path)
    _apForceLog(nameFile->pBuffer->str, nullptr);
}
