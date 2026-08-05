#include "apMEM_FRAME_DUMPER.h"
#include "../ds/dsCONST_ARRAY.h"

// Boundary rodata: apMEM_DL_ALLOC_TYPE -> label string, indexed by type. This is the global
// `str_0` @0x8420BB40 (a small const char*[] of dl-alloc-type labels).
extern const char *const apMemDlStatTypeName[];

// apMEM_FRAME_DUMPER::DumpDlInfo @ 0x82C4BD50 — private (AAA mangle).
// Walk the fixed dl-stat snapshot and, for every entry whose alloc category equals `type`, log it
// against the current dl running total and compact it out (overwrite the drained slot with the
// current last entry, then Remove to shift the tail down). Non-matching entries are skipped.
void apMEM_FRAME_DUMPER::DumpDlInfo(apMEM_DL_ALLOC_TYPE type)
{
    int nEntry = this->dlStat.nEntry;
    if (nEntry <= 0)
        return;

    int logicalIdx = 0;         // index handed to Remove / the IsValidIdx assert
    int slotIdx    = 0;         // element index into dlStat.list[]
    int lastIdx    = nEntry - 1; // current final live entry

    do
    {
        // Original repeats this IsValidIdx bounds assert around every list access; kept once.
        if (!IGNORE_STRONG_ASSERT && ((unsigned int)logicalIdx >= 0x20 || logicalIdx >= this->dlStat.nEntry))
            reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
                "IsValidIdx(idx)",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h",
                166,
                empty_string);

        if (this->dlStat.list[slotIdx].type == type)
        {
            int dlSize = this->sizeDl.size;
            this->Dump(this->nameLog, (char *)"%s, %10d,,, %40d\n",
                       apMemDlStatTypeName[type], this->dlStat.list[slotIdx].size, dlSize);

            // Overwrite the drained slot with the current last entry, then compact.
            this->dlStat.list[slotIdx].type = this->dlStat.list[lastIdx].type;
            this->dlStat.list[slotIdx].size = this->dlStat.list[lastIdx].size;
            this->dlStat.Remove(logicalIdx);
            --nEntry;
            --lastIdx;
        }
        else
        {
            ++logicalIdx;
            ++slotIdx;
        }
    }
    while (logicalIdx < nEntry);
}
