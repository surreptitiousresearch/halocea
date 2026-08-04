#pragma once
// ws-engine ds: stateless name-based comparator functor template, used as the CMP template
// argument to dsVECTOR<T,N>::Find when searching a vector of named records (e.g. vidDIP_CLASS,
// vidDIP, vidDIP_GROUP) by their embedded dsTSTRING<char> `name` field against a `const char*`
// key. DB type is an empty struct inheriting the equally-stateless dsNAME_COMPARE<N> (0 members
// either way -- types_members dsNAME_CMP<T,1,1>: 0 rows, just the dsNAME_COMPARE<1> base).

template<int CaseInsensitive>
struct dsNAME_COMPARE {
    // boundary -- name-vs-key equality predicate body external to this batch.
};

template<class T, int NameIsFirstMember, int CaseInsensitive>
struct dsNAME_CMP : dsNAME_COMPARE<CaseInsensitive> {
};
