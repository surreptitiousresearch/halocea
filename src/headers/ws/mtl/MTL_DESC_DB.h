#pragma once
#include "mtlMTL_MNG.h"
// ws-engine mtl: material-affix-name lookup/resolution database. Publicly derives from
// mtlMTL_MNG (single anonymous base member at offset 0 confirmed via types_members
// MTL_DESC_DB: one member, offset 0, size 1, type mtlMTL_MNG — no members of its own beyond
// the base). Selects/attaches per-split materials on an objOBJ tree from its instance affix
// string, resolving each split's material name via named ("vid.mtl"/"vid.mtl_strict") affixes
// or an inherited default. The process-wide instance lives at `mtlManager`/`mtlManagerInst`.

struct objOBJ;         // ../obj/objOBJ.h — pointer only
struct dsAFFIX_STRING; // ../ds/dsAFFIX_STRING.h

struct MTL_DESC_DB : mtlMTL_MNG {
    // 0x82A83B38 — recurse over `pObj` and its whole child tree (circular sibling list via
    // objOBJ::next, terminated back at objOBJ::child), invoking _SelectMaterialsByInstAffixes on
    // every node that has faces. Public entry point; thin wrapper over the private worker.
    void SelectMaterialsByInstAffixes(objOBJ *pObj, const dsTSTRING<char> &instAffixes);

    // 0x82A83680 — the real per-object implementation `SelectMaterialsByInstAffixes` forwards
    // to (verified via decompile: the public method calls this by name when pObj->nFace > 0).
    // Builds the combined affix string for `pObj`, resolves a base material name from it, then
    // for every split entry in pObj's geometry, looks up the entry's own material and rebuilds
    // its full name (base name + entry's material name + CRC32 suffix of the filtered affix
    // string) to select the matching slot in the corresponding vidORD split list. Protected
    // (mangle `AAA`).
    void _SelectMaterialsByInstAffixes(objOBJ *pObj, const dsTSTRING<char> &instAffixes); // protected

    // 0x82A82BA0 — resolve the base material name from an affix string: "vid.mtl_strict" or
    // "vid.mtl" affixes carry an explicit "material_name" parameter; "vid.mtl_strict" always wins
    // and, if it lacks the parameter, forces the empty string (no fallback). A "vid.mtl" match
    // appends a filtered-affix CRC32 suffix onto the extracted name; missing the parameter also
    // forces empty. Neither affix present -> empty string. Protected (mangle `AAA`), sret-by-value.
    dsTSTRING<char> _GetMtlNameFromAffix(const dsAFFIX_STRING &affixes); // protected

    // Select materials on `pObj` by explicit material name, or restore the object's default
    // materials. Invoked by entENTITY::implSetMaterial. boundary.
    void SelectMaterialsByMtlName(objOBJ *pObj, const dsTSTRING<char> &mtlName);
    void SelectMaterialsByDefault(objOBJ *pObj);
};

// Process-wide material manager instance (?mtlManager@@3PAVMTL_DESC_DB@@A). boundary.
extern MTL_DESC_DB *mtlManager;
