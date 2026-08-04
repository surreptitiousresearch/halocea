#pragma once
// ws-engine ds: a lightly-tagged pair used to give a struct's members readable names at the call
// site (`result.value` / `result.wasReplaced`) instead of `.first`/`.second`. Each tag wraps its
// payload type T by public inheritance so it is layout- and assignment-compatible with a bare T
// (ds::MAP::Insert assigns a raw MAP_ITERATOR straight into a NAMED_PAIR_value<MAP_ITERATOR>
// field — confirmed by the reversed ctor bodies).

namespace ds {

template<class T>
struct NAMED_PAIR_value : T {
    NAMED_PAIR_value() {}
    NAMED_PAIR_value(const T &v) : T(v) {}
    // 0x825568F0 (NAMED_PAIR_value<MAP_ITERATOR<int,msgADDR*,...>>) — copy ctor, bytewise copy.
    NAMED_PAIR_value(const NAMED_PAIR_value &other) : T(other) {}
};

template<class T>
struct NAMED_PAIR_wasReplaced {
    T wasReplaced;
    NAMED_PAIR_wasReplaced() {}
    NAMED_PAIR_wasReplaced(const T &v) : wasReplaced(v) {}
};

// Field-name tags for the (callback, data) NAMED_PAIR used by strmSTREAMING_QUEUE's
// per-queue update-notification slots. Each wraps its payload T as a single member (T may be a
// function pointer / void*, so member-wrap rather than inherit). Layout == sizeof(T).
template<class T>
struct NAMED_PAIR_callback {
    T callback;
    NAMED_PAIR_callback() {}
    NAMED_PAIR_callback(const T &v) : callback(v) {}
};

template<class T>
struct NAMED_PAIR_data {
    T data;
    NAMED_PAIR_data() {}
    NAMED_PAIR_data(const T &v) : data(v) {}
};

template<class A, class B>
struct NAMED_PAIR {
    A value;
    B wasReplaced;

    // 0x82557D28 (NAMED_PAIR<NAMED_PAIR_value<MAP_ITERATOR<int,msgADDR*,...>>,
    // NAMED_PAIR_wasReplaced<bool>>) — construct from the raw payload values (`tl` is the bare
    // wrapped type A derives from, e.g. a MAP_ITERATOR; `tr` is the bare bool B wraps).
    template<class TL>
    NAMED_PAIR(const TL &tl, const bool &tr) : value(tl), wasReplaced(tr) {}
};

} // namespace ds
