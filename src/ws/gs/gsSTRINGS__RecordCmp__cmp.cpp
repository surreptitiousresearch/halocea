#include "../../headers/ws/gs/gsSTRINGS.h"
#include "../ds/ds_assert_boundary.h"

// stricmp — CRT boundary (case-insensitive string compare), per project convention.
extern int stricmp(const char *a, const char *b);

// 0x826B80A8 — case-insensitive compare of record.ofsIdText's text (looked up via
// data->storageIds, the packed NUL-separated ID-text byte pool) against `key`.
int gsSTRINGS::RecordCmp::cmp(const gsSTRINGS::Record &record, const char *key) const
{
    int idTextOffset = record.ofsIdText;
    dsVECTOR<char, 8> *storageIds = &data->storageIds;

    if (!IGNORE_STRONG_ASSERT && (idTextOffset < 0 || idTextOffset >= storageIds->nElem))
    {
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            567,
            "idx", idTextOffset,
            "nElem", storageIds->nElem);
    }

    return stricmp(&storageIds->pData[idTextOffset], key);
}
