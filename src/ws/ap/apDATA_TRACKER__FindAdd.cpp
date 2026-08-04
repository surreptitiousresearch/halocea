#include "apDATA_TRACKER.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"
#include <string.h>

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free

// SR_REC_PRED — stateless name-ordering predicate functor for the record vector. The container
// FindSorted/InsertSorted specializations inline the actual byte-wise name compare; the only
// state is a zeroed 4-byte slot (the binary sets it to 0 before each call).
struct SR_REC_PRED { int reserved; };

// apDATA_TRACKER::FindAdd @ 0x82A25470 — private (AAA mangle).
// Return the index of the record named `key`. If it does not exist, build a fresh zero-valued
// record with that name, flag captions dirty, and insert it name-sorted (duplicates are a
// contract violation), returning the new index.
int apDATA_TRACKER::FindAdd(const char *key)
{
    SR_REC_PRED pred;
    pred.reserved = 0;

    const char *searchKey = key;
    int found = this->records.FindSorted<SR_REC_PRED, const char *>(searchKey, pred);
    if (found >= 0)
        return found;

    RECORD newRecord;                       // RECORD::RECORD() -> name adopts the empty singleton

    dsTSTRING<char> keyString;
    keyString.pBuffer = nullptr;
    keyString.UnsafeInit(key, -1, 0);
    newRecord.name = keyString;             // dsTSTRING::operator= (shares keyString's buffer)

    // release the temporary key string
    dsTSTRING_BUF_HEADER<char> *keyBuffer = keyString.pBuffer;
    if (keyBuffer->refCount-- == 1)
        dlFree(keyBuffer);

    memset(newRecord.values, 0, sizeof(newRecord.values));
    this->saveCaptions = true;

    pred.reserved = 0;
    int inserted = this->records.InsertSorted<SR_REC_PRED>(
        newRecord, pred, dsVECTOR<RECORD, 8>::INS_DUP_ERROR);

    // release the local record's name buffer (manual teardown of the temporary)
    dsTSTRING_BUF_HEADER<char> *recordBuffer = newRecord.name.pBuffer;
    if (recordBuffer->refCount-- == 1)
        dlFree(recordBuffer);

    return inserted;
}
