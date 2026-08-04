#pragma once
// ws-engine ai08: cached debug/display name strings for a brain.
// DB-verified layout (types_members aiNAMES): nameInst@0, nameClass@4, nameTpl@8 — size 12.

struct aiBRAIN; // ai08 — owning brain (fwd)

struct aiNAMES {
    const char *nameInst;  // 0x00 instance name
    const char *nameClass; // 0x04 class name
    const char *nameTpl;   // 0x08 template name

    // 0x8317891C-region call — populate the three name pointers from the brain's descriptor.
    // Body external to this batch. boundary.
    void Grab(aiBRAIN *pBrain);
};
