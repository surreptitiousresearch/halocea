#pragma once
#include "objFILTER.h"
// ws-engine obj: exact-name filter — objFind()/objFindName() predicate that accepts a node
// whose objOBJ::name matches `name` verbatim. DB-verified layout (types_members
// objFILTER_NAME): base objFILTER @0 (vtable ptr), name (const char *) @4 — size 8.

struct objFILTER_NAME : objFILTER {
    const char *name; // 0x04

    objFILTER_NAME(const char *name);        // 0x8267FE58 (??0objFILTER_NAME@@QAA@PBD@Z) boundary — not decompiled
    int IsUse(objOBJ *obj) override;         // 0x82680440 (?IsUse@objFILTER_NAME@@UAAHPAVobjOBJ@@@Z) boundary
    ~objFILTER_NAME() override;              // 0x8267FE70 (??1objFILTER_NAME@@UAA@XZ) boundary
};
