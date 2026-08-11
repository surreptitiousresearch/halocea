#include "../headers/ws/vid/vidDEFRAG_CHUNK_OBJ_HCEX_X360.h"
#include "../headers/ws/ds/ds_new.h"

// @0x823F1728 — ??R?$dsMakeFunctor@UvidDEFRAG_CHUNK_OBJ_HCEX_X360@@$0A@@@QBAPAUvidDEFRAG_CHUNK_OBJ_HCEX_X360@@PBDH@Z
// dsMakeFunctor<vidDEFRAG_CHUNK_OBJ_HCEX_X360,0>::operator()(const char *file, unsigned line): the
// branded-new factory the dscMAKE_FAMILY machinery calls to construct a chunk object on the
// tracked heap. Allocates 0x20 (32) bytes via the (size,file,line) placement operator new, then
// runs the default ctor (zero base fields + install vtable) — i.e. exactly
// `new(file,line) vidDEFRAG_CHUNK_OBJ_HCEX_X360()`. The functor carries no data, so `this` is
// unused; const method returning the freshly built object.
struct dsMakeFunctor_vidDEFRAG_CHUNK_OBJ_HCEX_X360 {
    vidDEFRAG_CHUNK_OBJ_HCEX_X360 *operator()(const char *file, unsigned int line) const;
};

vidDEFRAG_CHUNK_OBJ_HCEX_X360 *dsMakeFunctor_vidDEFRAG_CHUNK_OBJ_HCEX_X360::operator()(
    const char *file, unsigned int line) const
{
    return new (file, line) vidDEFRAG_CHUNK_OBJ_HCEX_X360();
}
