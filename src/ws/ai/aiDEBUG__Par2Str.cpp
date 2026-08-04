#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiDEBUG::Par2Str — the four static "<oper> : <value>" formatters used to build a debug console
// line. Each just sprintf's into a fresh dsTSTRING and returns it by value (sret in the ABI).
//
//   0x83194AA0  ?Par2Str@aiDEBUG@@SA?AV?$dsTSTRING@D@@PBDH@Z          (const char*, int)
//   0x83194AE8  ?Par2Str@aiDEBUG@@SA?AV?$dsTSTRING@D@@PBDM@Z          (const char*, float)
//   0x83194B30  ?Par2Str@aiDEBUG@@SA?AV?$dsTSTRING@D@@PBDAAUm3dV@@@Z  (const char*, m3dV&)
//   0x83194B98  ?Par2Str@aiDEBUG@@SA?AV?$dsTSTRING@D@@PBD0@Z          (const char*, const char*)

dsTSTRING<char> aiDEBUG::Par2Str(const char *oper, int value)
{
    dsTSTRING<char> result;
    dsSPrintf(&result, "%s : %d", oper, value);
    return result;
}

dsTSTRING<char> aiDEBUG::Par2Str(const char *oper, float value)
{
    dsTSTRING<char> result;
    dsSPrintf(&result, "%s : %.2f", oper, value);
    return result;
}

dsTSTRING<char> aiDEBUG::Par2Str(const char *oper, m3dV &value)
{
    dsTSTRING<char> result;
    dsSPrintf(&result, "%s : %.2f %.2f %.2f", oper, value.x, value.y, value.z);
    return result;
}

dsTSTRING<char> aiDEBUG::Par2Str(const char *oper, const char *value)
{
    dsTSTRING<char> result;
    dsSPrintf(&result, "%s : %s", oper, value);
    return result;
}
