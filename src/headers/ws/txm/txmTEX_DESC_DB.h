#pragma once
#include "ENGINE_TEX_DESC.h"
#include "../ds/dsTSTRING.h"
// ws-engine txm: process-wide table of the 10 default texture descriptors (one per streaming
// priority / suffix bucket -- see TEX_SUFFIXES_1), lazily loaded from "default_texdesc[_suffix].td".
// DB-verified layout (types_members txmTEX_DESC_DB): inited@0 (bool), default_desc@4
// (ENGINE_TEX_DESC[10]) -- size 564.

typedef struct txmTEX_DESC_DB
{
    bool            inited;           /* 0x000 */
    unsigned char _pad0[3]; /* db-verified padding */
    ENGINE_TEX_DESC default_desc[10]; /* 0x004 */

    // 0x82C66404 -- default ctor (zero-inits inited/default_desc). Body not decompiled in this
    // batch. boundary.
    txmTEX_DESC_DB();

    // 0x82C66C24 -- reversed in txmTEX_DESC_DB__Init.cpp. One-shot: loads "default_texdesc.td"
    // (index 0) and "default_texdesc_<TEX_SUFFIXES_1[i]>.td" (index 1..9) via psSystem, falling
    // back to ENGINE_TEX_DESC::Default() on a parse failure. No-op if already inited. The
    // psSYSTEM::LoadPsFromFile call remains a boundary (its sret/root argument shuffle is a
    // stack-reuse artifact -- modeled as psSYSTEM__LoadPsFromFile in txm_texdesc_db_init_boundary.h).
    void Init();

    // 0x82C66624 -- copy the loaded default_desc[i] matching `name`'s streaming-priority suffix
    // into *desc (falls back to index 0 if no suffix matches). Body external to this batch.
    // boundary.
    void GetDefaultDesc(ENGINE_TEX_DESC *desc, const dsTSTRING<char> &name);

    // 0x82C66E70 -- resolve a texture's on-disk ".td" sidecar (named after `name`) and load it,
    // falling back to GetDefaultDesc on a miss. Body external to this batch. boundary.
    void GetTexDesc(ENGINE_TEX_DESC *desc, const dsTSTRING<char> &name);
} txmTEX_DESC_DB;

// Per-suffix ".td" filename fragments consulted by Init() (index 0 is unused -- index 0 loads
// the bare "default_texdesc.td"). Not reversed as data in this batch -- boundary raw string table.
extern "C" const char *TEX_SUFFIXES_1[];

// Process-wide singleton instance. boundary (definition site external to this batch).
extern txmTEX_DESC_DB txmTexDescDB;
