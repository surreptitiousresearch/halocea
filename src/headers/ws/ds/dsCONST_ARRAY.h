#pragma once
#include "ds_assert_boundary.h"
// ws-engine ds: fixed-capacity (N) inline array with a live-entry count. Elements are stored
// by value directly after the count; no heap backing.
// DB-verified layout (types_members dsCONST_ARRAY<apMEM_DL_STAT,32>):
//   nEntry@0 (int), list@4 (T[N]).

template<class T, int N>
struct dsCONST_ARRAY {
    int nEntry;  // 0x00 number of live entries
    T   list[N]; // 0x04 inline element storage

    // Default ctor — needed by consumers that default-construct the array (the count-taking ctor
    // below otherwise suppresses the implicit default). Default-constructs the inline slots.
    dsCONST_ARRAY() {}

    // (dsCONST_ARRAY<hcexJOB_SYNC_INST,4>) — count-taking ctor: set the live-entry count and
    // default-construct all N inline slots. (ds_const_list.h)
    dsCONST_ARRAY(int nEntry);

    // 0x823CFA70 (dsCONST_ARRAY<hcexJOB_SYNC_INST,4>) — true when idx is in [0, N) AND below the
    // live-entry count. (ds_const_list.h:166 asserts on this before operator[].)
    int IsValidIdx(int idx) const;

    // 0x82C4B8A0 (dsCONST_ARRAY<apMEM_DL_STAT,32>) — reference to the last live entry
    // (index nEntry-1); asserts 0 < nEntry <= N first.
    T &Back();

    // 0x82C4B920 (dsCONST_ARRAY<apMEM_DL_STAT,32>) — append a copy of `val` into the next
    // inline slot, incrementing the entry count; asserts nEntry < N first.
    void Add(const T &val);

    // 0x82C4BBA0 (dsCONST_ARRAY<apMEM_DL_STAT,32>) — remove the entry at `idx` by shifting the
    // trailing entries down over it, clearing the vacated last slot, then decrementing the count.
    // No-op if idx is out of [0, N) or beyond the live count.
    void Remove(int idx);

    // (dsCONST_ARRAY<jbmTHREAD,16>) — indexed access into the inline storage; no bounds
    // check here (callers that need one do it themselves, e.g. jbmMANAGER::WaitThreadsDone's
    // IsValidIdx STRONG_ASSERT before this call).
    T &operator[](int idx);

    // const overload (mangled QBA..., returns `const T &`) — indexed read of the inline storage
    // from a const array (e.g. vidMEM_MNG::GetDynHwB's const access to curDynBuf).
    const T &operator[](int idx) const;

    // 0x823BFDC8 (dsCONST_ARRAY<int,64>) — shift the live entries at/after `pos` up by `num` slots
    // and copy `num` values from `begin` into the vacated range, then advance nEntry by `num`.
    // Used by hcex_collect_crc to push a new CRC onto the front (pos 0) of the history ring.
    void Insert(int pos, const T *begin, int num);

    // 0x8273FA50 (dsCONST_ARRAY<m3dV,10>) — reset the array to a single live entry holding `fill`
    // (nEntry := 1, list[0] := fill). Used by aiVIS_RESULTS::FakeFill.
    void Clear(const T &fill);
};
