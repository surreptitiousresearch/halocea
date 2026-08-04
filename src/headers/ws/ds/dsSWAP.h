#pragma once
// ws-engine ds: stateless element-exchange policy used by dsVECTOR<T,N>::Sort / HeapSort / Sisp.
// DB-verified: dsSWAP<dsAFFIX_BUILDER::AFFIX> carries no data members (empty functor). Callers
// invoke it to exchange two elements; in the reversed Sort/Sisp bodies the actual element move is
// emitted inline (dsSWAP is passed by const-ref but only its type participates).

template<class T>
struct dsSWAP {
    // Exchange the two elements `a` and `b`. // boundary — inlined at every call site in Sort/Sisp.
    void operator()(T &a, T &b) const;
};
