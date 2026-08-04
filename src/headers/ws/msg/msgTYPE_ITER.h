#pragma once
#include "msgSYSTEM.h"

// ws-engine msg: a type-filtered iterator over the message system's registered actors. Wraps a
// msgSYSTEM::ITERATOR and skips forward to the next endpoint whose runtime type is T. Constructed
// from msgSYSTEM::Front(). Boundary template for this wave — the skip/advance bodies live in the
// msg subsystem and are the next frontier; only the interface the ai08 loops use is declared.
// DB member layout observed in callers: a single msgSYSTEM::ITERATOR `it` at offset 0.

template<class T>
struct msgTYPE_ITER {
    msgSYSTEM::ITERATOR it; // 0x00 underlying bucket/position iterator

    // Construct from a raw system iterator, advancing to the first actor of type T. boundary.
    msgTYPE_ITER(const msgSYSTEM::ITERATOR &start);

    T   *Get() const;                    // current T (null at end)          boundary
    T   *operator->() const;             // boundary
    bool IsDone() const;                 // true when past the last T         boundary
    msgTYPE_ITER &operator++();          // advance to the next T             boundary
};
