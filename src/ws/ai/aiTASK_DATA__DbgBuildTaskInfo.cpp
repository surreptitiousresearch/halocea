// aiTASK_DATA::DbgBuildTaskInfo @ 0x832612B8
// ?DbgBuildTaskInfo@aiTASK_DATA@@QAAXAAV?$dsTSTRING@D@@ABV2@@Z
#include "../../headers/ws/ai/aiTASK_DATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern const char byte_820D3084[]; // shared field-separator string @0x820D3084

// Non-virtual: append this task's full debug line to `out`. Layout is:
//   <runTag> <sep> <type> <sep> <info> <sep> <info2> <sep> <"[BLOCKER]" or " ">
// A separator is only emitted when `out` is currently non-empty (matches the source guards).
void aiTASK_DATA::DbgBuildTaskInfo(dsTSTRING<char> &out, const dsTSTRING<char> &runTag)
{
    out += runTag;
    if (out.pBuffer->strLen)
        out.Insert(out.pBuffer->strLen, byte_820D3084, -1);

    DbgPrintType(&out);
    if (out.pBuffer->strLen)
        out.Insert(out.pBuffer->strLen, byte_820D3084, -1);

    DbgPrintInfo(&out);
    if (out.pBuffer->strLen)
        out.Insert(out.pBuffer->strLen, byte_820D3084, -1);

    DbgPrintInfo2(&out);
    if (out.pBuffer->strLen)
        out.Insert(out.pBuffer->strLen, byte_820D3084, -1);

    if (IsBlocker())
        out.Insert(out.pBuffer->strLen, "[BLOCKER]", -1);
    else
        out.Insert(out.pBuffer->strLen, " ", -1);
}
