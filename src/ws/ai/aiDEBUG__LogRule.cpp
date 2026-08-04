#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiNAMES.h"

// aiDEBUG::LogRule @ 0x831969F8  (?LogRule@aiDEBUG@@IAAXPBDH0@Z)
//
// Emit a "~Rules~" trace line for the rule operation `oper`, tagged with the rule name derived from
// the source file path (`file`) and the source line. Gated by the rules-enabled / watchee-only
// debug vars.
void aiDEBUG::LogRule(const char *file, int line, const char *oper)
{
    if (!dbg_isRulesEnabled.value)
        return;
    if (dbg_isRulesWatcheeOnly.value && this->pBrain != _pBrainDebugWatchee)
        return;

    dsTSTRING<char> path;
    path.UnsafeInit(file, -1, 0);
    int len = path.pBuffer->strLen;
    const char *s = path.pBuffer->str;

    // Rule name = the token between the last path-separator / underscore and the extension dot.
    int idxSlash = -1;
    for (int i = len - 1; i >= 0; --i) { if (s[i] == '\\') { idxSlash = i; break; } }
    int idxUnder = -1;
    for (int i = len - 1; i >= 0; --i) { if (s[i] == '_') { idxUnder = i; break; } }
    if (idxSlash > idxUnder)
        idxUnder = idxSlash;
    int start = idxUnder + 1;
    if (start < 0)                 // (((v12>>31)-1)&v12): clamp negatives to 0
        start = 0;

    int idxDot = -1;
    for (int i = len - 1; i >= 0; --i) { if (s[i] == '.') { idxDot = i; break; } }
    if (idxDot < 0)
        idxDot = len - 1;

    path = path.SubStr(start, idxDot - start);

    dsTSTRING<char> ruleName;
    dsSPrintf(&ruleName, "%s-%d", path.pBuffer->str, line);

    // Varargs packing on PPC is decompiler-ambiguous; args reproduced in call order: the brain's
    // instance name (channel tag + quoted name), the formatted operation, and the rule name.
    _apLog("~Rules,%s~'%s': %-20s %s",
           this->pBrain->names.nameInst, this->pBrain->names.nameInst, oper, ruleName.pBuffer->str);
}
