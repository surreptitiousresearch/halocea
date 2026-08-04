#pragma once
// ws-engine nav — a named waypoint set / "front" (the geometry a squad front resolves to).
// BOUNDARY: the full navWP_SET data layout is the next frontier. Only the query methods that the
// aiSQUAD front-distance code (aiSQUAD::implGetDistMemberToFront / implGetDistMemberToFrontTarget)
// dispatches on a navWP_SET* are declared here; the object is used by pointer only, so an
// incomplete body is safe. Method decls only (no competing full body).

struct navWP;

struct navWP_SET {
    // Defined elsewhere (nav) — bodies are the next frontier.  boundary
    int    GetNWP() const;                 // number of waypoints in the set
    navWP *GetWP(int idx);                 // idx-th waypoint
    bool   IsContainWP_ID(short wpId);     // true when wpId belongs to this set
};
