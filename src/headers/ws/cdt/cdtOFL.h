#pragma once
#include "../ap/apSTATE_T.h"
// ws-engine cdt (collision-detection): an "object face list" scratch buffer used by collision
// broad-phase queries to collect candidate object/face ids. DB-verified layout
// (types_members cdtOFL): nID@0, idList@4, nID2List@8, startID2List@12, nID2@16, id2List@20,
// nIDMax@24, nID2Max@28, state@32 (apSTATE_T<unsigned long>) — size 36.

typedef struct cdtOFL {
    int                        nID;          // 0x00 live id count
    unsigned short            *idList;       // 0x04 primary id list (nIDMax entries)
    unsigned short            *nID2List;     // 0x08 secondary id list, per-primary-slot count
    unsigned int              *startID2List; // 0x0C secondary id list, per-primary-slot start offset
    int                        nID2;         // 0x10 live secondary id count
    unsigned short            *id2List;      // 0x14 secondary id storage (nID2Max entries)
    int                        nIDMax;       // 0x18 capacity of idList
    int                        nID2Max;      // 0x1C capacity of id2List/nID2List/startID2List
    apSTATE_T<unsigned long>   state;        // 0x20

    // 0x827E4F24 — zero state + the 8 following words (nID.._u? — matches the same whole-object
    // zero-loop pattern as cdtINFO's ctor), then allocate idList (nID ushorts). If nID2 is 0, mark
    // state bit 0x1 (single-level mode) and set nIDMax=nID/nID2Max=0. Otherwise also allocate
    // nID2List (nID ushorts), startID2List (nID uints) and id2List (nID2 ushorts); nIDMax/nID2Max
    // are only committed once every allocation succeeds.
    cdtOFL(unsigned int nID, unsigned int nID2);

    // 0x827E4D54 — free idList/nID2List/startID2List/id2List (each only if non-null).
    ~cdtOFL();

    // 0x827E4DA8 — append other's 1st-level id list (clamped to nIDMax).
    // REVERSED: src/ws/cdt/cdtOFL__Merge1stLevel.cpp.
    void Merge1stLevel(cdtOFL &other);
    // 0x827E4E10 — append other's 2nd-level id list (clamped to nID2Max).
    // REVERSED: src/ws/cdt/cdtOFL__Merge2ndLevel.cpp.
    void Merge2ndLevel(cdtOFL &other);
} cdtOFL;
