#pragma once
#include "voxGRID.h"
#include "../ds/PTR_LIST.h"
#include "../ds/dsNODE_CACHE.h"
#include "../ds/dsBIT_ARRAY.h"
#include "../ap/apSTATE_T.h"
// ws-engine cdt: dynamic-collider spatial index — a voxel grid over per-instance node lists.
// DB-verified layout (types_members cdtCOLL_DYN, size 88 / 0x58):
//   __vftable@0, idMax@4, voxGrid@8 (voxGRID, 48), voxNodeList@0x38
//   (ds::PTR_LIST<cdtCOLL_DYN_NODE,...>*), pNodeCache@0x3C (dsNODE_CACHE<cdtCOLL_DYN_NODE>*),
//   idProcessedList@0x40 (dsBIT_ARRAY, 16), pGenData@0x50, state@0x54 (apSTATE_T<unsigned long>).

struct cdtCOLL_DYN_NODE;      // cdt subsystem — per-instance collider node (pointer only)  boundary
struct cdtCOLL_DYN_vtbl;      // boundary — cdt subsystem vtable

struct cdtCOLL_DYN {
    cdtCOLL_DYN_vtbl                                                    *__vftable;       // 0x00
    int                                                                  idMax;           // 0x04
    voxGRID                                                              voxGrid;         // 0x08
    ds::PTR_LIST<cdtCOLL_DYN_NODE, ds::LIST_NODE_ACCESS_POLICY<cdtCOLL_DYN_NODE> > *voxNodeList; // 0x38 per-voxel node lists
    dsNODE_CACHE<cdtCOLL_DYN_NODE>                                      *pNodeCache;      // 0x3C
    dsBIT_ARRAY                                                          idProcessedList; // 0x40
    void                                                                *pGenData;        // 0x50
    apSTATE_T<unsigned long>                                             state;           // 0x54
}; // 88 bytes
