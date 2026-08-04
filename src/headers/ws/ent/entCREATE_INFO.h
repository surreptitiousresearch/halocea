#pragma once
#include "../ds/dsTSTRING.h"
#include "../msg/msgDATA.h"
// ws-engine ent: parameter block passed to entENTITY::Register / entENTITY::OnRegister when
// spawning/registering an entity.
// DB-verified layout (types_members entCREATE_INFO) — size 24.

struct entCREATE_DATA; // ent subsystem — creation-data block (pointer only)   boundary

typedef struct entCREATE_INFO {
    dsTSTRING<char>  nameTpl;    // 0x00 entity template name
    dsTSTRING<char>  nameClass;  // 0x04 entity class name
    entCREATE_DATA  *pCD;        // 0x08 creation-data block
    msgDATA         *pInitData;  // 0x0C message-system init payload, forwarded to msgADDR::Register
    unsigned int     mpID;       // 0x10 multiplayer unique-id override (-1 = none/unset)
    int              uid;        // 0x14

    // 0x82533DB8 — build a create-info block from raw class/template names: `_nameClass` is
    // adopted verbatim; `_nameTpl` is adopted verbatim ONLY if non-null and non-empty, otherwise
    // the template name defaults to entGetClassTplName(_nameClass) (the class's configured
    // default template). `pCD`/`pInitData` are stored as-is; `mpID`/`uid` default to -1 (unset).
    // Reversed in the entCreate batch.
    entCREATE_INFO(const char *_nameClass, const char *_nameTpl,
                   entCREATE_DATA *_pCD, msgDATA *_pInitData);

    // 0x825331C8 — default construct: empty nameTpl/nameClass, null pCD/pInitData, mpID/uid = -1.
    // Same-class ctor called by the entCreate(char*,animINST*,msgDATA*) overload; not decompiled
    // in this batch. boundary.
    entCREATE_INFO(); // boundary

    // 0x82532260 — release nameTpl/nameClass's shared string buffers. Not reversed in this
    // batch. boundary.
    ~entCREATE_INFO(); // boundary
} entCREATE_INFO;
