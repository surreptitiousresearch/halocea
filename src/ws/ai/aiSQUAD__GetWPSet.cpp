#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiSQUAD::GetWPSet(aiCOORD_GROUP*, const dsTSTRING<char>&, dsTSTRING<char>&) const @ 0x832A9660
// ?GetWPSet@aiSQUAD@@MBAPAVnavWP_SET@@PAVaiCOORD_GROUP@@ABV?$dsTSTRING@D@@AAV4@@Z
//
// Resolve the named front / waypoint-set `name` from the coordinated group's nav system. Each
// failure path writes a human-readable reason into `hintErr` and returns null; success clears
// `hintErr` to the empty string and returns the WP-set.
navWP_SET *aiSQUAD::GetWPSet(aiCOORD_GROUP *grp, const dsTSTRING<char> &name,
                            dsTSTRING<char> &hintErr) const
{
    navSYS *navSystem = grp->GetNavSystem();
    if (!navSystem)
    {
        hintErr = dsTSTRING<char>("Can't find squad nav system");
        return nullptr;
    }

    if (!navSystem->IsWpSetReady(name))
    {
        hintErr = dsTSTRING<char>("Can't get front at first frame - front not ready");
        return nullptr;
    }

    navWP_SET *wpSet = navSystem->GetWpSet(name);
    if (!wpSet)
    {
        hintErr = dsTSTRING<char>("Can't find front");
        return nullptr;
    }

    hintErr = dsTSTRING<char>(""); // byte_8200155A
    return wpSet;
}
