#pragma once
#include "dsPARAM_LIST.h"
#include "dsPARAM.h"
#include "dsSTRID.h"
// ws-engine ds: a fixed-capacity parameter list — a dsPARAM_LIST view (length/list) whose backing
// storage is an inline dsPARAM[N] rather than a heap vector. Used for small, statically-bounded
// argument bundles handed to dsEVENT_MGR::SignalEvent and the ds_data property system.
// DB-verified layout (types_members dsSTATIC_PARAM_LIST<1>/<2>): <dsPARAM_LIST base>@0 (8 bytes),
// storage@8 (dsPARAM[N], 12 bytes each).

template<int N>
struct dsSTATIC_PARAM_LIST : dsPARAM_LIST {
    dsPARAM storage[N]; // 0x08 inline element storage (N entries)

    // 0x823D0788 (<2>) — default-construct each inline dsPARAM (empty id + null-type value), then
    // publish the base view (list = &storage[0], length = 0). Reversed per-instantiation.
    dsSTATIC_PARAM_LIST();

    // 0x823D0720 (<2>) — destroy each inline dsPARAM whose value is non-empty (dispatch the stored
    // type's Destroy virtual, vtable slot 3). Reversed per-instantiation.
    ~dsSTATIC_PARAM_LIST();

    // Append a parameter named `id` holding `val` at storage[length], then bump length. The value
    // slot was left empty by the constructor, so this is a plain SetValue (no prior Destroy).
    // 0x823D84D8 (<2>, dsTSTRING<char>). Reversed per-instantiation.
    template<class T>
    void Add(dsSTRID id, const T &val);

    // Same as above but interning the name `id` into a dsSTRID first.
    // 0x823D7B10 (<1>, dsTSTRING<char>). Reversed per-instantiation.
    template<class T>
    void Add(const char *id, const T &val);
};
