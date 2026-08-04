#include "../../headers/ws/ai/aiHO_SYS_BASE.h"

// aiHO_SYS_BASE::FindHOClosestVis @ 0x83179C20
// ?FindHOClosestVis@aiHO_SYS_BASE@@UAAFFAAV?$dsVECTOR@F$07@@FAAF@Z
//
// Base default stub: the "closest visible HO" query just returns the first candidate of `fromArr`
// (asserted non-empty) and echoes `wpidTo`. wpidParent is ignored.
short aiHO_SYS_BASE::FindHOClosestVis(short wpidParent, dsVECTOR<short, 8> &fromArr,
                                     short wpidTo, short &wpidFrom)
{
    (void)wpidParent;
    wpidFrom = fromArr[0];
    return wpidTo;
}
