#pragma once
#include "psSECTION.h"
#include "psSECTION_KEY_REF.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsDATA.h"
// ws-engine ps: iterator over the keys/records of a section, optionally filtered by name/type.
// DB-verified layout (types_members psITERATOR): name@0 (dsTSTRING<char>), type@4 (psSTD_TYPEID),
// flags@8 (unsigned int), records@12 (dsVECTOR<psSECTION_KEY_REF,8>), pos@32 (int) — size 36.

// DB-verified enum (types_enum_values psSTD_TYPEID).
enum psSTD_TYPEID {
    PS_TYPEID_INVALID = -1,
    PS_TYPEID_NULL    = 0,
    PS_TYPEID_INT     = 1,
    PS_TYPEID_FLOAT   = 2,
    PS_TYPEID_BOOL    = 3,
    PS_TYPEID_STRING  = 4,
    PS_TYPEID_OLD     = 5,
    PS_TYPEID_ARRAY   = 6,
    PS_TYPEID_SECTION = 7,
};

typedef struct psITERATOR {
    dsTSTRING<char>            name;    // 0x00 name filter (empty = no name filter)
    psSTD_TYPEID                type;    // 0x04 type filter (PS_TYPEID_NULL = no type filter)
    unsigned int                flags;   // 0x08
    dsVECTOR<psSECTION_KEY_REF, 8> records; // 0x0C matching key refs collected at Begin()
    int                          pos;     // 0x20 current index into `records`

    // 0x823D6BEC-adjacent — collect every key of `ps` matching `_type`, with the given `_flags`.
    // Reversed only via this call site; ctor body itself is out of scope for this batch. boundary
    psITERATOR(psSECTION ps, psSTD_TYPEID _type, unsigned int _flags);

    // 0x8251B600 (??0psITERATOR@@QAA@XZ) — default ctor: empty, unfiltered, positioned before any
    // records (name adopts the empty-string singleton; records built with the ps_section.cpp cookie).
    psITERATOR();

    // 0x823D4E80 (??1psITERATOR@@QAA@XZ) — destroy the record vector and release the name buffer.
    ~psITERATOR();

    // ?GetValue@psITERATOR@@QBA?AVdsDATA@@XZ — the current record's boxed value (sret). Body
    // external to this batch. boundary.
    dsDATA GetValue() const;

    int  IsDone() const;                       // 0x82998FF8 — pos >= records.nElem
    void Next();                               // 0x82998FF8-adjacent — advance pos
    int  GetKeyIdx() const;                    // records[pos].keyIdx
    psSECTION *GetKeySection(psSECTION *result) const; // records[pos].ps (sret)
    dsTSTRING<char> *GetStr(dsTSTRING<char> *result, dsTSTRING<char> def) const; // sret; boundary

    // Current record's section handle (added by the HALO_SOUND_PARAMS re-source pass; used by
    // HALO_SOUND_PARAMS::Init's PS_TYPEID_SECTION "replace" sub-section loop). sret. boundary.
    psSECTION *GetSection(psSECTION *result) const;
} psITERATOR;
