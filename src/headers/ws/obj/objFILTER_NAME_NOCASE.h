#pragma once
#include "objFILTER.h"
// ws-engine obj: case-insensitive exact-name filter — objFind() predicate that accepts a node
// whose objOBJ::name matches `name` ignoring case. DB-verified layout (types_members
// objFILTER_NAME_NOCASE): base objFILTER @0 (vtable ptr), name (const char *) @4 — size 8.
// NOTE: per the DB it derives objFILTER directly (not objFILTER_NAME) and declares its own
// `name` member, even though the layout is byte-identical to objFILTER_NAME.

struct objFILTER_NAME_NOCASE : objFILTER {
    const char *name; // 0x04

    objFILTER_NAME_NOCASE(const char *name); // 0x823CE930 (??0objFILTER_NAME_NOCASE@@QAA@PBD@Z) boundary
    int IsUse(objOBJ *obj) override;         // 0x823CE980 (?IsUse@objFILTER_NAME_NOCASE@@UAAHPAVobjOBJ@@@Z) boundary
    ~objFILTER_NAME_NOCASE() override;       // 0x823CE9E0 (??1objFILTER_NAME_NOCASE@@UAA@XZ) boundary
};
