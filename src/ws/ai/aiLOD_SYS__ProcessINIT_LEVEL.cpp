#include "../../headers/ws/ai/aiLOD_SYS.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h" // ParseSpline
#include "../../headers/ws/ds/dsTSTRING.h"

// aiLOD_SYS::ProcessINIT_LEVEL @ 0x83239CA8
// ?ProcessINIT_LEVEL@aiLOD_SYS@@QAAXXZ
//
// Per-level init: parse the default distance->LOD response curve (a lazily-initialised static
// spline-text string) into this system's runtime spline. Curve control points are
// (distance;LOD): 0;0 5;0 6;1 10;2 30;6 100;100 888;100.
void aiLOD_SYS::ProcessINIT_LEVEL()
{
    static dsTSTRING<char> defaultLODCurve("0;0|5;0|6;1|10;2|30;6|100;100|888;100");
    aiGOAL_BASE::ParseSpline(&defaultLODCurve, &curveLODValue);
}
