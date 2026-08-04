#pragma once
#include "psSECTION.h"
#include "psSECTION_RECORD.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsDATA.h"
struct psCOMPLEX_KEY_DATA;  // parsed complex key (full layout in psCOMPLEX_KEY_DATA.h)
struct psSECTION_KEY_REF;   // located key reference (full layout in psSECTION_KEY_REF.h)
// ws-engine ps: the ref-counted body of a config/property section — its interned name, owner,
// parent list, sorted parameter records, and branch back-pointer.
// DB-verified layout (types_members psSECTION_DATA):
//   refCount@0, name@4 (dsSTRID), pOwner@8 (psSECTION_DATA*), psParents@12 (dsVECTOR<psSECTION,8>),
//   orderedRecords@32 (dsVECTOR<psSECTION_RECORD,8>), pBranch@52 (psSECTION_BRANCH*),
//   state@56 (unsigned __int8) — size 57.

struct psSECTION_BRANCH; // owning branch (full layout in psSECTION_BRANCH.h)

typedef struct psSECTION_DATA {
    int                            refCount;       // 0x00
    dsSTRID                        name;           // 0x04 interned section name
    psSECTION_DATA                *pOwner;         // 0x08 owning (parent) section body
    dsVECTOR<psSECTION, 8>         psParents;      // 0x0C parent-section handles
    dsVECTOR<psSECTION_RECORD, 8>  orderedRecords; // 0x20 sorted parameter records
    psSECTION_BRANCH              *pBranch;        // 0x34 owning branch
    unsigned char                  state;          // 0x38

    psSECTION_DATA();                                      // boundary — default construct (empty body)

    // ??0psSECTION_DATA@@QAA@ABV0@@Z (0x8251F480) — deep-copy construct.
    psSECTION_DATA(const psSECTION_DATA &data);

    // ?RemoveKey@psSECTION_DATA@@QAAHKAAUpsCOMPLEX_KEY_DATA@@@Z (0x8251B774) — remove the key
    // named by the last segment of `c`'s key chain.
    int RemoveKey(unsigned int flags, psCOMPLEX_KEY_DATA &c);

    // 0x82518EE0 — key name at record index `idx`; empty dsSTRID when out of range. const (QBA).
    dsSTRID GetKeyName(int idx) const;

    // 0x82518F40 — value at record index `idx` (empty dsDATA when out of range). const (QBA).
    dsDATA GetKeyValue(int idx) const;

    // 0x8251A6E0 — parent section handle at index `idx` (empty when out of range). const (QBA).
    psSECTION GetParent(int idx) const;

    // 0x8251A000 — drop one reference; runs a pending rescan and frees the branch at zero.
    void Release();

    // 0x8251B6C8 — remove `psParent` from this section's parent list, then release the handle.
    void RemoveParent(psSECTION psParent);

    // 0x8251... (?GetSubSection@psSECTION_DATA@@IAAPAV1@H@Z) — sub-section body at index `idx`.
    psSECTION_DATA *GetSubSection(int idx);                // boundary (protected, IAA)

    // ?SetOwner@psSECTION_DATA@@QAAXPAV1@@Z — set the owner section body.
    void SetOwner(psSECTION_DATA *owner);                  // boundary

    // 0x8251C508 (?AddParent@psSECTION_DATA@@QAAXVpsSECTION@@@Z) — append `parent` to the parent
    // list (removing any existing entry for the same body first, then fixing branch refcounts).
    void AddParent(psSECTION parent);

    // 0x8251A768 (?FindRecThisOnly@psSECTION_DATA@@IAAHVdsSTRID@@@Z) — index of the record whose key
    // equals `key` in THIS section only (no parent walk), or -1. Protected (IAA). Lazily re-sorts
    // the record table when the sort-dirty flag (state&8) is set.
    int FindRecThisOnly(dsSTRID key);

    // 0x8251CBC8 (?GetValue@psSECTION_DATA@@QAA?AVdsDATA@@KAAUpsCOMPLEX_KEY_DATA@@@Z) — resolve the
    // parsed `complexKey` against this section and return the located key's value (empty dsDATA when
    // absent). Also records the last-resolved key name / line on psSECTION_INTERFACE. sret return.
    dsDATA GetValue(unsigned int flags, psCOMPLEX_KEY_DATA &complexKey);

    // 0x8251AD20 (?RemoveRec@psSECTION_DATA@@QAAXH@Z) — remove record `idx`, renumber the remaining
    // records' sort predecessors, mark the table sort-dirty, and (for sub-section values) detach the
    // removed sub-section's owner.
    void RemoveRec(int idx);

    // 0x8251B868 (?AddRec@psSECTION_DATA@@IAAHVdsSTRID@@VdsDATA@@KH@Z) — append a record {`key`,
    // `obj`} at source line `lineNmb`. When (flags&1) replaces an existing record for `key`;
    // otherwise fails (-1) if `key` already exists. Returns the new record index or -1. Protected
    // (IAA); consumes the by-value `obj`. For sub-section values, wires up the child's owner.
    int AddRec(dsSTRID key, dsDATA obj, unsigned int flags, int lineNmb);

    // 0x8251C5C0 (?AddKey@psSECTION_DATA@@QAAHVdsDATA@@KHAAUpsCOMPLEX_KEY_DATA@@@Z) — install `obj`
    // under the parsed `complexKey`, descending/creating intermediate sub-sections for a dotted key.
    // Returns the leaf record index or -1. Consumes the by-value `obj`.
    int AddKey(dsDATA obj, unsigned int flags, int lineNmb, psCOMPLEX_KEY_DATA &complexKey);

    // 0x8251C860 (?GetValueRefs@psSECTION_DATA@@QAAHKPAV?$dsVECTOR@VpsSECTION@@$07@@PAUpsSECTION_KEY_REF@@AAUpsCOMPLEX_KEY_DATA@@@Z)
    // — resolve the parsed key chain `c` against this section, collecting located key references.
    // When `pRefArr` is given, every matching section handle is appended; when `pOneRef` is given,
    // the first match is written there. Descends dotted sub-sections, then walks parent sections and
    // the owner scope. Returns 1 when a single-ref lookup was satisfied, else 0.
    int GetValueRefs(unsigned int flags, dsVECTOR<psSECTION, 8> *pRefArr,
                     psSECTION_KEY_REF *pOneRef, psCOMPLEX_KEY_DATA &c);

    ~psSECTION_DATA();                                     // boundary
} psSECTION_DATA;
