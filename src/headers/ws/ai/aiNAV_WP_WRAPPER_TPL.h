#pragma once
// ws-engine ai08: template-actor nav waypoint wrapper. DB-verified layout
// (types_members aiNAV_WP_WRAPPER_TPL) — size 0x10: aiNAV_WP_WRAPPER_GS@0 (8B base, opaque here),
// parentEntity@8, doAutolink@0xC. Constructed by aiPLANNER::CreateNavWPWrapperTpl.

struct navMANAGER;      // nav subsystem      boundary (ptr)
struct entENTITY;       // entENTITY.h
struct aiNAV_WP_DATA;   // aiNAV_WP_DATA.h

// DB-verified base layout: aiNAV_WP_WRAPPER_GS { aiNAV_WP_WRAPPER@0 (4B, opaque), navManager@4 }.
struct aiNAV_WP_WRAPPER_GS {
    unsigned char aiNAV_WP_WRAPPER[4]; // 0x00 gs wrapper base (opaque boundary)
    navMANAGER   *navManager;          // 0x04

    // Add/update a waypoint from `data` under the fully-resolved name `name`; returns its id.
    // boundary — body external to this batch.
    short ChangeOrAdd(const aiNAV_WP_DATA &data, const char *name);
};

struct aiNAV_WP_WRAPPER_TPL {
    aiNAV_WP_WRAPPER_GS base;             // 0x00 nav-gs wrapper base (navManager at +4)
    entENTITY    *parentEntity;           // 0x08
    bool          doAutolink;             // 0x0C
    unsigned char _padD[3];               // 0x0D

    // 0x... — bind to `nm`'s nav graph for `ent`. boundary — body external to this batch.
    aiNAV_WP_WRAPPER_TPL(navMANAGER *nm, entENTITY *ent);

    // 0x8321A668 — add (or update) a waypoint from `data` named `nameWP` (name-scoped by the
    // parent instance); binds it to the parent's model matrix and optionally autolinks. Returns
    // the waypoint id, or -1 on failure. Delegates the name-resolved add to aiNAV_WP_WRAPPER_GS.
    int ChangeOrAdd(const aiNAV_WP_DATA &data, const char *nameWP);

    // 0x8321ABB0 — default-fill `data` for waypoint index `idx`. Returns 0.
    int Get(int idx, aiNAV_WP_DATA &data) const;
};
