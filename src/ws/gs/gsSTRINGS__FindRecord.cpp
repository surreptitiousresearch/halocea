#include "../../headers/ws/gs/gsSTRINGS.h"

// 0x826B91A0 — binary-search mapTextIdToIdx for the record whose ID text case-insensitively
// matches idText, via RecordCmp::cmp.
//
// DEVIATION: the decompiler emitted 12 extra `int a3..a13` parameters between idText and a
// trailing duplicate `const char *a14`. Disassembly (826b91a0-826b91dc) shows the function body
// only ever touches r3 (this) and r4 (idText) — it copies r3/r4 into its own stack frame, builds
// a RecordCmp on the stack, and tail-calls dsVECTOR<Record,8>::FindSorted. The phantom params are
// a decompiler stack-frame-sharing artifact (confirmed against the sibling prototype
// gsSTRINGS__FindRecordPart(gsSTRINGS*, const char*) in `funcs`, which has the correct 2-arg
// shape). The real signature is (gsSTRINGS *this, const char *idText).
int gsSTRINGS::FindRecord(const char *idText)
{
    gsSTRINGS::RecordCmp cmp;
    cmp.data = this;

    return mapTextIdToIdx.FindSorted<gsSTRINGS::RecordCmp, const char *>(idText, cmp);
}
