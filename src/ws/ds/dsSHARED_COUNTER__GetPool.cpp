// dsSHARED_COUNTER::GetPool @0x82B8F8A8 — lazily heap-allocate the process-wide recycled-slot
// pool on first use, caching it in the static `pool` global thereafter.
#include "../../headers/ws/ds/dsSHARED_COUNTER.h"
void *operator new(size_t size, const char *file, unsigned int line); // boundary — debug-tagged new (Saber)

static ds::VAL_LIST<dsSHARED_COUNTER> *pool;

ds::VAL_LIST<dsSHARED_COUNTER> *dsSHARED_COUNTER::GetPool()
{
    if (!pool) {
        ds::VAL_LIST<dsSHARED_COUNTER> *created =
            (ds::VAL_LIST<dsSHARED_COUNTER> *)operator new(sizeof(ds::VAL_LIST<dsSHARED_COUNTER>),
                                                             "D:\\Projects\\code\\common\\src.sys\\ds\\ds_list.cpp", 0x15);
        if (created) {
            created->usedHead = -1;
            created->usedTail = -1;
            created->freeHead = -1;
            created->pool.pData = nullptr;
            created->pool.nElem = 0;
            created->pool.allocated = 0;
            created->pool.__cl.file = "D:\\Projects\\code\\common\\src.sys\\ds\\ds_list.cpp";
            created->pool.__cl.line = 21;
            created->cl = created->pool.__cl;
        }
        pool = created;
    }
    return pool;
}
