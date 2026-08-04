#pragma once
// Boundary declaration of bhvPATH — the "walk a nav path" behaviour (ai08). Only the members reached
// by the aiBRAIN MoveTo* re-source are declared: the HO-selection predicates whose member-function
// pointers are stored in bhpPATH_PAR::useHOPath. The full bhvPATH data layout is the next frontier.

struct navWP; // nav waypoint (fwd — passed by const ref)

// bhvPATH_USEHO_RET — HO-selection result enum passed by reference to the UseHO_* predicates.
// Enumerators are the next frontier; only the reference is taken here.
enum bhvPATH_USEHO_RET : int;

struct bhvPATH {
    // 0x83270998 (?UseHO_CombatStyle@bhvPATH@@QBA_NABVnavWP@@AAW4bhvPATH_USEHO_RET@@@Z) — decide,
    // combat-style, whether to route the path through the hideout waypoint `wp`. boundary.
    bool UseHO_CombatStyle(const navWP &wp, bhvPATH_USEHO_RET &ret) const;
};

// The member-function-pointer type stored in bhpPATH_PAR::useHOPath (the path's HO predicate).
typedef bool (bhvPATH::*bhvPATH_UseHOFn)(const navWP &, bhvPATH_USEHO_RET &) const;
