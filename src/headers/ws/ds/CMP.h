#pragma once
// ws-engine ds: ds::CMP — stateless default comparator policy (empty struct, size 1; DB
// types_members ds::CMP has no data members). Static template methods are the default
// comparator used by FindNonSorted / MAP when no explicit CMP is supplied; each instantiation
// is a distinct static fn (bodies not decompiled — boundary). Shared by ds::MAP and ds::LIST
// (both previously defined it locally → C2011 when co-included).

namespace ds {
typedef struct CMP {
    template<class T, class U> static bool equals(const T &a, const U &b); // boundary
    template<class T, class U> static int  cmp(const T &a, const U &b);    // boundary
} CMP;
} // namespace ds
