#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/ws/ps/psSYSTEM.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

extern "C" void osOutputDebugString(const char *fmt, ...); // boundary — os debug log

extern "C" char *strchr(const char *s, int c); // boundary — CRT

// psITERATOR — Blam ".ps" tag-script section iterator; a sibling of the parser subsystem this
// method touches. Only the subset of its interface LoadTagParams calls is declared here
// (construct-by-typeid, IsDone/GetSection/Next); body belongs to the ps-parser subsystem, not
// reversed in this batch. boundary.
struct psITERATOR {
    psITERATOR(const psSECTION &section, int typeId, unsigned int flags); // boundary
    ~psITERATOR();                                                          // boundary
    bool IsDone() const;                                                    // boundary
    psSECTION &GetSection(psSECTION *result) const;                        // boundary
    void Next();                                                            // boundary
};
enum { PS_TYPEID_SECTION = 0 }; // boundary — value used at the sole call site below

namespace ds {
// ds::Sort<T*, CMP>(T *first, T *last, CMP *cmp) / ds::RemoveDuplicates<T, CMP>(dsVECTOR<T,8>
// *vec, CMP *cmp) — generic range-sort and adjacent-duplicate-removal algorithms used across
// the ds:: template drain (shared with plain dsVECTOR<T,8>::Sort()); declared here with the
// exact pointer-to-comparator calling convention the disasm shows at this call site. boundary.
template<class T, class CMP> void Sort(T *first, T *last, CMP *cmp);
template<class T, class CMP> int  RemoveDuplicates(dsVECTOR<T, 8> *vec, CMP *cmp);
} // namespace ds

// HALO_SOUND_SYSTEM::LoadTagParams() @ 0x836C3468
// Parses "d:\sounds\xbox360\tags_params.ps" and populates `params`/`paramsWildcard` (a tag
// name containing '*' goes to the wildcard table, everything else to the exact-match table),
// then sorts both and asserts no duplicate tag names slipped through.
// CAVEAT: the ps-parser plumbing (psSYSTEM/psITERATOR/INCLUDE_PREPROCESSOR) is a large sibling
// subsystem; this reconstruction keeps its calls at face value (construct/iterate/dispose)
// without descending into their bodies, matching the project's boundary convention for
// same-tier sibling subsystems.
void HALO_SOUND_SYSTEM::LoadTagParams()
{
    dsTSTRING<char> path;
    path.UnsafeInit("d:\\sounds\\xbox360\\tags_params.ps", -1, 0);

    dsTSTRING<char> name;
    name.UnsafeInitEmpty();
    dsTSTRING<char> errHint;
    errHint.UnsafeInitEmpty();

    psSYSTEM ps;
    ps.multipleAccessGroups = 0;
    ps.macro.pData = nullptr;
    ps.macro.nElem = 0;
    ps.macro.allocated = 0;
    ps.includePreprocessor = nullptr;
    ps._LoadPsFromFile(psSystem, path, name, errHint);

    if (!ps.multipleAccessGroups) {
        osOutputDebugString("[Sound Error] Failed to load \"%s\"\n", "d:\\sounds\\xbox360\\tags_params.ps");
        return;
    }

    // DEVIATION: disasm constructs the root section via
    // `psSECTION::psSECTION(&localSection, (const psSECTION*)&ps)` -- i.e. reinterpreting the
    // whole `psSYSTEM` object as a `psSECTION`. This works because both structs' first 4 bytes
    // are a single handle field (psSYSTEM::multipleAccessGroups doubles as the parsed root
    // section's handle once `_LoadPsFromFile` has succeeded). Reproduced here as an explicit
    // reinterpret rather than inventing a named conversion helper.
    psSECTION rootSection(*reinterpret_cast<const psSECTION *>(&ps));
    psITERATOR iter(rootSection, PS_TYPEID_SECTION, 0);
    while (!iter.IsDone()) {
        psSECTION section;
        iter.GetSection(&section);

        HALO_SOUND_PARAMS entry;
        entry.Init(section);

        bool isWildcard = strchr(entry.tag.CStr(), '*') != nullptr;
        if (isWildcard)
            paramsWildcard.PushBack(entry);
        else
            params.PushBack(entry);

        iter.Next();
    }

    HALO_SOUND_PARAMS::CMP cmp{};
    ds::Sort(params.Begin(), params.End(), &cmp);
    if (!IGNORE_STRONG_ASSERT) {
        HALO_SOUND_PARAMS::CMP dupCmp{};
        if (ds::RemoveDuplicates(&params, &dupCmp))
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "ds::RemoveDuplicates(params, HALO_SOUND_PARAMS::CMP()) == 0",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                2262, empty_string);
    }
}
