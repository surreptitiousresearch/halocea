#pragma once

namespace ds_data {

// Per-T storage-construction / value-lifetime policy for the ds_data value model. The `storage`
// word is the 4-byte value cell; for a heap-boxed type T it holds a pointer to a heap T and these
// statics allocate / copy / free through it. The dsDATA_TYPE_IMPL<T> and dsDATA_TYPE_STATIC<T>
// forwarders delegate value lifetime and pointer access here.
//
// This is the single canonical definition. It previously had a body in THREE headers
// (ws/ds/dsDATA_TYPE_HELPERS.h, hcex/halo_ds_data_type_boundary.h and here) with three different
// member sets and two different cell spellings -- `error: redefinition` in the header_layout probe
// TU, and a live hazard because which one a TU saw depended on which header it happened to include.
// The union of the three is below, adjudicated against the DB's mangled names (`names` table):
//
//   ?Construct@?$dsDATA_TYPE_CONSTRUCT@U<T>@@@ds_data@@SAXPAKABU<T>@@@Z   @ 0x823BE5A0 (PLAYER_ACTIONS)
//   ?CopyConstruct@...@ds_data@@SAXPAKPBK@Z                               @ 0x823BE658
//   ?Destroy@...@ds_data@@SAXPAK@Z                                        @ 0x823BBFA0
//   ?Copy@...@ds_data@@SAXPBKPAK@Z                                        @ 0x823BBFE8
//   ?CopyObj@...@ds_data@@SAXPBU<T>@@PAK@Z                                @ 0x823BC040
//   ?GetPtr@...@ds_data@@SAPAU<T>@@PAK@Z                                  @ 0x823BC090
//   ?GetPtr@...@ds_data@@SAPBU<T>@@PBK@Z                                  @ 0x823BC0B0
//
// Two DEVIATIONS from those manglings, both ABI-neutral and both pre-existing corpus spellings
// kept so the reversed specializations stay valid:
//  * the cell is spelled `unsigned int *` where the mangling says `K` = `unsigned long *`; both are
//    the same 4-byte cell on this target and `unsigned int` is what all ten reversed
//    dsDATA_TYPE_CONSTRUCT_*.cpp bodies and the IMPL/STATIC forwarders use.
//  * `Construct` takes `const T *` where the mangling says `ABU<T>` = `const T &`; an address in a
//    register either way, and the four reversed call sites pass
//    `&dsDATA_DEFAULT_VALUE<T>::GetDefaultVal()`.
// The former ws/ds/dsDATA_TYPE_HELPERS.h spelling declared the cell as `void **` (its own comment
// recorded that the mangled type was really `unsigned long*`); that spelling is gone and the three
// haloPLAYER_ACTIONS_ENV bodies that used it were re-spelled to match their haloRAW_INPUT_X360_ENV
// twins. GetPtr's return was `void *` here (wrong per SAPAU<T>/SAPBU<T>) and is now T*/const T*,
// which is what the IMPL GetPtr forwarders' own comments already said they relied on.
template<class T>
struct dsDATA_TYPE_CONSTRUCT {
    static void Construct(unsigned int *storage, const T *defVal);              // construct slot from an object
    static void CopyConstruct(unsigned int *storage, const unsigned int *data); // construct slot from a slot
    static void Destroy(unsigned int *storage);                                 // free the boxed T, null the slot
    static void Copy(const unsigned int *src, unsigned int *dest);              // overwrite boxed T from another box
    static void CopyObj(const T *obj, unsigned int *dest);                      // overwrite boxed T from a raw object
    static T       *GetPtr(unsigned int *storage);        // boundary -- box accessor/allocator
    static const T *GetPtr(const unsigned int *storage);  // boundary -- const twin
};

} // namespace ds_data
