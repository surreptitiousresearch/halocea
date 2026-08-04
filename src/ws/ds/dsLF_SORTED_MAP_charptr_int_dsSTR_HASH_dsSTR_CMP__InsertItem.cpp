#include <string.h>
#include "dsLF_SORTED_MAP.h"
#include "ds_assert_boundary.h"

struct dsSTR_HASH; // boundary — const char* hashing policy
struct dsSTR_CMP;  // boundary — const char* ordering policy

extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line); // boundary — dlmalloc allocator

// dsLF_SORTED_MAP<const char*,int,dsSTR_HASH,dsSTR_CMP>::InsertItem @ 0x8261A6E8
// Allocate a new chain node one item longer than `line`, copy `line`'s items across a gap opened
// at `index`, store `item` into the gap, hand the old node to the deferred-erase manager, and
// return the new node.
template<>
dsLF_SORTED_MAP<const char *, int, dsSTR_HASH, dsSTR_CMP>::LINE *
dsLF_SORTED_MAP<const char *, int, dsSTR_HASH, dsSTR_CMP>::InsertItem(
        LINE *line, int index, const ITEM *item)
{
    if (!IGNORE_STRONG_ASSERT) {
        int length = line ? line->length : 0;
        if (index < 0 || index > length)
            STRONG_ASSERT_DUMMY().Crash(
                "index >= 0 && index <= ((line == 0) ? 0 : line->length)",
                "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_lf_map.h",
                308,
                dsStrongAssertMessage);
    }

    int newLength = line ? line->length + 1 : 1;
    LINE *newLine = (LINE *)dlMalloc(8 * newLength + 4, this->cl.file, this->cl.line);
    newLine->length = newLength;

    if (line) {
        memcpy(newLine->items, line->items, 8 * index);
        memcpy(&newLine->items[index + 1], &line->items[index], 8 * (line->length - index));

        LINE_ERASER eraser;
        eraser.line = line;
        eraser.index = -1;
        GetEraseMgr()->SafelyEraseObject(eraser);
    }

    // The decompiler emits a never-null check on the destination slot address; kept for fidelity.
    if (&newLine->items[index] != 0) {
        newLine->items[index].key = item->key;
        newLine->items[index].val = item->val;
    }
    return newLine;
}
