#pragma once
#include "strmPACK_BASE.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsCMP.h"
#include "../os/osLOCK.h"
// ws-engine strm (streaming): a single-file "pak" container (a .s3d — Saber 3D pack) holding many
// named sub-resources, each described by a PAK_RECORD. DB-verified layout (types_members
// strmPACK_S3D): base strmPACK_BASE@0, fileAsync@4 (fioSTRM_ASYNC_FILE), recTable@552, shortName@572
// (char[260]), initial@832, cached@833, dbgName@836, reopenName@840, lock@844 — size 896.

#include "../fio/fioSTRM_ASYNC_FILE.h"

struct strmPACK_S3D : strmPACK_BASE {
    // strmPACK_S3D::PAK_RECORD — one catalog entry in the pak's directory table.
    // DB-verified layout (types_members strmPACK_S3D::PAK_RECORD) — size 28.
    struct PAK_RECORD {
        unsigned int offset;  // 0x00 byte offset of the sub-resource within the pak file
        unsigned int size;    // 0x04 byte size of the sub-resource
        dsSTRID      name;    // 0x08 interned resource name (the FindPakRec search key)
        int          pakId;   // 0x0C sub-pak/language variant selector
        char         lang[4]; // 0x10
        char         subLang[4]; // 0x14
        int          idRec;   // 0x18 record index (self-referential, set once resolved)

        // strmPACK_S3D::PAK_RECORD::CMP — the (name, pakId) ordering comparator used by
        // dsVECTOR<PAK_RECORD,8>::FindSorted in FindPakRec. DB-verified: derives from (empty)
        // dsCMP, adds pkId@0 (EBO — the base occupies no separate storage).
        struct CMP : dsCMP {
            int pkId;

            CMP();          // 0x828EF9A8 — default ctor (boundary)
            CMP(int pakId); // 0x828EF9B8 — sets pkId = pakId (boundary)
        };
    };

    fioSTRM_ASYNC_FILE  fileAsync;           // 0x004 (548B, see ../fio/fioSTRM_ASYNC_FILE.h)
    dsVECTOR<PAK_RECORD, 8> recTable;        // 0x228 (552)
    char                 shortName[260];     // 0x23C (572)
    bool                 initial;            // 0x340 (832)
    bool                 cached;             // 0x341 (833)
    unsigned char        _pad342[2];         // 0x342 db-verified padding
    dsTSTRING<char>      dbgName;            // 0x344 (836)
    dsTSTRING<char>      reopenName;         // 0x348 (840)
    osLOCK               lock;               // 0x34C (844)

    strmPACK_S3D(const char *shortName); // 0x82677638 — boundary (outside this re-source)

    // 0x828F1B80 — binary-search recTable for the record matching (name, pakId); asserts the
    // resolved record's interned name matches on a hit. Returns the record's index, or -1.
    int FindPakRec(const dsSTRID &name, int pakId);

    bool CanOpenFile(const dsSTRID &resName, int pakId, bool *cached); // boundary (outside this re-source)
    void CloseFile(fioFILE *file);                                    // boundary (outside this re-source)
};
