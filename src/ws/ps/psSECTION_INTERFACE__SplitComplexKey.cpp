/* ?SplitComplexKey@psSECTION_INTERFACE@@ABAHPBDHHAAUpsCOMPLEX_KEY_DATA@@@Z @0x82518B70 */
#include "psSECTION_INTERFACE.h"
#include <string.h>
#include "psCOMPLEX_KEY_DATA.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTATIC_VECTOR.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"

// 0x82518B70 — parse a dotted config key ("a.b.c") into `c`'s interned sub-key chain. Each
// '.'-delimited segment before the last dot is interned into a dsSTRID and appended; when
// `internExistingOnly` is set an unknown segment yields a null id and the whole parse fails (0).
// The trailing segment (after the last '.', or the whole key when there is no '.') is either
// interned like the rest, or — when `lastIsSubString` is set — stored verbatim into `c._psSubStr`
// with an empty-string strid used as the leaf key. Finally the assembled chain is reversed in place
// so it is ordered leaf-first, as consumed by psSECTION_DATA::GetValueRefs. Returns 1 on success.
// Private const helper (`this` is unused).

// 0x8200155A — interned empty-string literal used to build an empty dsSTRID. boundary.
extern const char psEmptyStringLiteral[];
// Debug allocator free, used inline by the reversed dsTSTRING buffer release. boundary.
extern "C" void dlFree(void *ptr);

int psSECTION_INTERFACE::SplitComplexKey(const char *key, int internExistingOnly,
                                         int lastIsSubString, psCOMPLEX_KEY_DATA &c) const
{
    dsSTRID internedKey; // accumulates the interned id of the current segment (starts null)

    // Clear any prior key chain. (dsSTRID has a trivial destructor, so the original per-element
    // teardown loop is a no-op and collapses to resetting the count.)
    c._psKeysArr.nElem = 0;

    // Intern each segment that precedes a '.' and append it to the chain.
    for (const char *dotPtr = strchr(key, '.'); dotPtr; dotPtr = strchr(dotPtr + 1, '.')) { // strchr(const char*) yields const char*
        dsTSTRING<char> segment;
        segment.pBuffer = nullptr;
        segment.UnsafeInit(key, dotPtr - key, 0);

        dsSTRID segmentId(segment.pBuffer->str, internExistingOnly != 0);
        internedKey.id = segmentId.id;

        // Release the temporary segment buffer.
        if (segment.pBuffer->refCount-- == 1)
            dlFree(segment.pBuffer);

        // A segment that failed to intern (null id under exist-only lookup) fails the whole parse.
        dsSTRID emptyId; // default-constructed sentinel (id == nullptr)
        if (emptyId.id == internedKey.id)
            return 0;

        c._psKeysArr.PushBack(internedKey);
        key = dotPtr + 1;
    }

    // Handle the trailing segment (after the last '.', or the whole key when there was no dot).
    const dsSTRID *leafToPush;
    dsSTRID leafKey;
    if (lastIsSubString) {
        // Store the trailing text verbatim as the section's sub-string; use an empty-string strid
        // as the leaf key.
        dsTSTRING<char> tail;
        tail.pBuffer = nullptr;
        tail.UnsafeInit(key, -1, 0);
        c._psSubStr = tail; // ref-counted assignment: drops the old buffer, shares tail's
        if (tail.pBuffer->refCount-- == 1)
            dlFree(tail.pBuffer);

        leafKey = dsSTRID(psEmptyStringLiteral, 1);
        leafToPush = &leafKey;
    } else {
        leafKey = dsSTRID(key, internExistingOnly != 0);
        internedKey.id = leafKey.id;
        dsSTRID emptyId;
        if (emptyId.id == internedKey.id)
            return 0;
        leafToPush = &internedKey;
    }
    c._psKeysArr.PushBack(*leafToPush);

    // Reverse the assembled chain in place (leaf-first ordering).
    int nElem = c._psKeysArr.nElem;
    int halfCount = nElem >> 1;
    if (halfCount > 0) {
        dsSTRID *front = c._psKeysArr.pData;
        dsSTRID *back = &c._psKeysArr.pData[nElem];
        for (int swapIndex = 0; swapIndex < halfCount; ++swapIndex) {
            const char *frontId = front->id;
            front->id = back[-1].id;
            --back;
            back->id = frontId;
            ++front;
        }
    }
    return 1;
}
