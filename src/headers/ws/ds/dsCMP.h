#pragma once
// ws-engine ds: stateless default comparator functor (empty struct, size 1). Used as the
// CMP template argument to dsVECTOR::FindSorted for the pooled name->uid sorted tables.
// DB: types_members dsCMP has no data members.

typedef struct dsCMP {
} dsCMP;
