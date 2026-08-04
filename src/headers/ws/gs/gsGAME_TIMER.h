#pragma once
// ws-engine gs: a game timer with an optional expiry callback. Only the ClearCB entry point is
// reached by the ai batch (aiBRAIN::RemoveSSLCrossRefs drops SSL-timer callbacks on teardown);
// the full layout is a boundary (the next frontier).

struct gsGAME_TIMER {
    // 0x82DAE460 (?ClearCB@gsGAME_TIMER@@QAAXXZ) — detach any expiry callback from this timer.
    // boundary.
    void ClearCB();
};
