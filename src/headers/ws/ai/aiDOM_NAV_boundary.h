#pragma once
// ws-engine nav domain (aiDOM_NAV = gsDOMAIN base + isReady). BOUNDARY: the full gsDOMAIN graph
// (iaIACTOR@0, scnDOMAIN@0xC8, SSL_SYNC_OBJECT@0x124, ...) is the next frontier. Only the
// scnDOMAIN-owned model-object pointer that aiCOORDINATOR::AddGroupFrontsMesh reads is exposed.
// pObj offset is disasm-verified (lwz r27,0xCC(r3) at 0x83237064), not asserted as the full layout.

struct objOBJ; // obj-subsystem model object (pointer only) boundary

struct aiDOM_NAV {
    unsigned char _base[0xCC]; // 0x00 gsDOMAIN prefix (iaIACTOR/scnDOMAIN/...) — boundary
    objOBJ       *pObj;        // 0xCC front/domain model object (waypoint-set geometry)
    // remaining body (isReady@0x170, ...) omitted — pointer-only usage, next frontier
};
