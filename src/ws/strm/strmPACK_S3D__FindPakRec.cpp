#include "../../headers/ws/strm/strmPACK_S3D.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// strmPACK_S3D::FindPakRec @ 0x828F1B80
// Binary-search recTable (ascending by PAK_RECORD::CMP, i.e. by pakId) for the record matching
// `name`; on a hit, asserts the resolved record's interned name actually matches (a corrupt/
// stale sort would otherwise silently return the wrong record).
int strmPACK_S3D::FindPakRec(const dsSTRID &name, int pakId)
{
    PAK_RECORD::CMP cmp(pakId);
    int idx = recTable.FindSorted<PAK_RECORD::CMP, dsSTRID>(name, cmp);

    if (idx >= 0 && !IGNORE_STRONG_ASSERT && name.id != recTable[idx].name.id) {
        STRONG_ASSERT_DUMMY().Crash("recTable[id].name == name",
            "D:\\Projects\\code\\common\\src.sys\\drv\\strm_resource.cpp", 305,
            empty_string);
    }

    return idx;
}
