#pragma once
// ws-engine anim instance — BOUNDARY slice. The full animINST (legacy Blam anim graph) is a large
// type not re-sourced in this wave; only the fields the ai08 SMR helpers read are exposed. Offsets
// are DB-verified (types_members animINST): name@0x28, pPhysSystem@0xB4, pObj@0x148. Leading fields
// are byte-accurate padding, NOT an authoritative field-by-field layout. animINST is used through a
// pointer only, so the tail past pObj is intentionally unmodelled.

struct physSYSTEM_BASE; // phys system (pointer only)   boundary (fwd)
struct objOBJ;          // obj model object (pointer only) boundary (fwd)
struct m3dOBB;          // oriented bounding box          boundary (fwd)

struct animINST {
    unsigned char    _pad0[0x28];         // 0x00
    char            *name;                // 0x28 instance name (may be null)
    unsigned char    _pad1[0xB4 - 0x2C];  // 0x2C
    physSYSTEM_BASE *pPhysSystem;         // 0xB4 physics system, if any
    unsigned char    _pad2[0x148 - 0xB8]; // 0xB8
    objOBJ          *pObj;                // 0x148 root model object

    // ?GetOBB@animINST@@... — instance-space oriented bounding box (bone index; 0 = whole instance).
    m3dOBB *GetOBB(int boneIndex);

    // animINST::GetPos — write the instance's world-space origin into `out`.
    void GetPos(struct m3dV *out) const;
};
