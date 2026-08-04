#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dV.h"
// ws-engine prop subsystem: named parent/child skin-attachment override table for a preset tree
// (instCONSTRUCTOR_NODE). Each RELATION overrides the local transform instCONSTRUCTOR_NODE::
// SetRelations applies between a named ancestor object and a named child object.
// DB-verified layout (types_members instCONSTRUCTOR_NODE_RELATIONS): name@0 (dsTSTRING<char>),
// relations@4 (dsVECTOR<RELATION,8>, 20B) — size 24.
// DB-verified layout (types_members instCONSTRUCTOR_NODE_RELATIONS::RELATION): parent@0
// (dsTSTRING<char>), child@4 (dsTSTRING<char>), translation@8 (m3dV), rotation@20 (m3dV),
// scale@32 (m3dV) — size 44.

typedef struct instCONSTRUCTOR_NODE_RELATIONS {
    struct RELATION {
        dsTSTRING<char> parent;      // 0x00 parent/ancestor object name
        dsTSTRING<char> child;       // 0x04 child object name
        m3dV            translation; // 0x08 override translation
        m3dV            rotation;    // 0x14 override Euler rotation (degrees)
        m3dV            scale;       // 0x20 override scale
    };

    dsTSTRING<char>       name;      // 0x00 preset name this relation table belongs to
    dsVECTOR<RELATION, 8> relations; // 0x04

    // 0x826DD3E8 — find the RELATION entry matching the named parent/child pair, or null if no
    // override is registered for that pair. Body external to this batch. boundary.
    RELATION *GetRelation(const dsTSTRING<char> &parent, const dsTSTRING<char> &child);
} instCONSTRUCTOR_NODE_RELATIONS;
