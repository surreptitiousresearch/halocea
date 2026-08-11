/* Flattened boundary definitions for the DBG_CINE_INFO record's compiler-generated lifecycle
 * (ctor/dtor/operator=) plus its private Init() helper, as declared in DBG_CINE_INFO.h. Bodies
 * reproduce the decompiled ws-engine methods (addresses noted per function). The mapActors sub-object
 * lifecycle (its own ds::MAP ctor/copy-ctor and the node-pool Term) is one bounded-descent level
 * below and is left as boundary externs; its Clear is the flat sibling in DBG_CINE_INFO_mapActors.cpp. */

#include "../headers/hcex/DBG_CINE_ACTOR_MAP_NODE.h" /* DBG_CINE_INFO, mapActors view, flat map ops */
#include "../headers/hcex/haloENGINE_CONTROL.h"      /* haloENGINE_CONTROL::curLevel */
#include "../headers/hcex/hcex_dbgvar_boundary.h"    /* dbg_hcex_CineMakeSave, dbgVAR_MANAGER_* */
#include "headers/game_time_constants.h"

/* --- boundary externs (one bounded-descent level below these methods) --- */
/* ds::MAP<...>::MAP(apCL) @ 0x823B8FD8 — default-construct an empty map tagged with call-site cl. */
extern void DBG_CINE_actor_map_ctor(void *map, const char *clFile, int clLine);
/* ds::MAP<...>::MAP(const MAP&) — copy-construct (rehashes every source node into a fresh table). */
extern void DBG_CINE_actor_map_copyctor(void *dst, const void *src);
/* dsNODE_CACHE<DUMMY>::Term — free every owned node block and reset the pool. */
extern void dsNODE_CACHE_Term(void *allocator);

extern int   osFileMakeDir(const char *path);         /* ws os: create the capture directory tree */
extern "C" void main_save_map_nonsafe(void);         /* Blam bridge: force a checkpoint save */
extern float gsElapsedTimeCineDump;                   /* cine-dump frame clock (-1 idle, 1/30 armed) */
extern haloENGINE_CONTROL *haloEngineCtrl;            /* Blam engine control object */

/* DBG_CINE_INFO::DBG_CINE_INFO @ 0x823B9630 — default-construct: empty path/name (shared singleton),
 * clock zeroed, not inited, and a fresh empty mapActors tagged with the halo_cine.cpp call site. */
void DBG_CINE_INFO_ctor(DBG_CINE_INFO *self)
{
    self->path.pBuffer = 0;
    dsTSTRING_UnsafeInitEmpty(&self->path);
    self->name.pBuffer = 0;
    dsTSTRING_UnsafeInitEmpty(&self->name);
    self->timeFromStart = 0.0f;
    self->isInited = 0;
    DBG_CINE_actor_map_ctor(&self->mapActors,
                            "D:\\Projects\\code\\HCEX\\sources\\halo_cine.cpp", 0xA3);
}

/* DBG_CINE_INFO::~DBG_CINE_INFO @ 0x823B9750 — Clear the map, Term its node pool, free the pool's
 * node-block list and bucket vector, then drop the name/path buffer references. */
void DBG_CINE_INFO_dtor(DBG_CINE_INFO *self)
{
    DBG_CINE_INFO_mapActors_Clear(&self->mapActors);

    DBG_CINE_ACTOR_MAP_VIEW *map = (DBG_CINE_ACTOR_MAP_VIEW *)&self->mapActors;
    dsNODE_CACHE_Term(&map->allocator);
    dlFree(map->allocator.nodeList.pData);
    dlFree(map->data.pData);

    dsTSTRING_BUF_HEADER<char> *nameBuf = self->name.pBuffer;
    if (nameBuf->refCount-- == 1)
        dlFree(nameBuf);

    dsTSTRING_BUF_HEADER<char> *pathBuf = self->path.pBuffer;
    if (--pathBuf->refCount == 0)
        dlFree(pathBuf);
}

/* DBG_CINE_INFO::operator= @ 0x823BA930 — share the other record's path/name buffers, copy the
 * scalars, then tear down and copy-construct the mapActors sub-object. */
DBG_CINE_INFO *DBG_CINE_INFO_assign(DBG_CINE_INFO *self, const DBG_CINE_INFO *other)
{
    if (other != self)
    {
        dsTSTRING_BUF_HEADER<char> *pathBuf = self->path.pBuffer;
        if (pathBuf->refCount-- == 1)
            dlFree(pathBuf);
        self->path.pBuffer = other->path.pBuffer;
        ++self->path.pBuffer->refCount;
    }

    if (&other->name != &self->name)
    {
        dsTSTRING_BUF_HEADER<char> *nameBuf = self->name.pBuffer;
        if (--nameBuf->refCount == 0)
            dlFree(nameBuf);
        self->name.pBuffer = other->name.pBuffer;
        ++self->name.pBuffer->refCount;
    }

    self->isInited = other->isInited;
    self->timeFromStart = other->timeFromStart;
    self->frame = other->frame;

    if (&self->mapActors != &other->mapActors)
    {
        DBG_CINE_INFO_mapActors_Clear(&self->mapActors);
        DBG_CINE_ACTOR_MAP_VIEW *map = (DBG_CINE_ACTOR_MAP_VIEW *)&self->mapActors;
        dsNODE_CACHE_Term(&map->allocator);
        dlFree(map->allocator.nodeList.pData);
        dlFree(map->data.pData);
        DBG_CINE_actor_map_copyctor(&self->mapActors, &other->mapActors);
    }

    return self;
}

/* DBG_CINE_INFO::Init @ 0x823BAA80 — private helper (arm the capture): optionally push the cine name
 * into the checkpoint-name debug var and force a save, build the per-cinematic output directory
 * "CINE_DUMP\<level>\<name>" into `path`, create it, resetting the whole record on a mkdir failure,
 * and arm the dump clock. */
void DBG_CINE_INFO_Init(DBG_CINE_INFO *self)
{
    if (dbg_hcex_CineMakeSave.value)
    {
        dsTSTRING<char> chptVarName;
        chptVarName.pBuffer = 0;
        chptVarName.UnsafeInit("0HCEX/checkpoints/name", -1, 0);

        dbgVAR_MANAGER *mgr = dbgVAR_MANAGER_GetManager();
        dbgVAR *var = dbgVAR_MANAGER_FindFirst(mgr, &chptVarName);
        if (var && var->typeId == 4)
            dbgVAR_MANAGER_SetDbgVarValue_tstring(mgr, var, &self->name);

        dsTSTRING_BUF_HEADER<char> *chptBuf = chptVarName.pBuffer;
        if (--chptBuf->refCount == 0)
            dlFree(chptBuf);

        main_save_map_nonsafe();
    }

    self->isInited = 1;

    /* Build "CINE_DUMP\<curLevel>\<name>". The raw decompile inlines each append as the protected
     * Insert(pos,len) gap-open + memcpy; expressed here via the equivalent public Insert overloads. */
    haloENGINE_CONTROL *ctrl = haloEngineCtrl;
    dsTSTRING<char> path;
    path.pBuffer = 0;
    path.UnsafeInit("CINE_DUMP", -1, 0);
    path.Insert(path.pBuffer->strLen, "\\", -1);
    path.Insert(path.pBuffer->strLen, ctrl->curLevel);
    path.Insert(path.pBuffer->strLen, "\\", -1);
    path.Insert(path.pBuffer->strLen, self->name);

    self->path = path; /* release old buffer, share the built one (matches the inlined ref swap) */

    if (!osFileMakeDir(self->path.pBuffer->str))
    {
        DBG_CINE_INFO_mapActors_Clear(&self->mapActors);
        gsElapsedTimeCineDump = -1.0f;

        DBG_CINE_INFO fresh;
        DBG_CINE_INFO_ctor(&fresh);
        DBG_CINE_INFO_assign(self, &fresh);
        DBG_CINE_INFO_dtor(&fresh);
    }

    self->frame = 0;
    self->timeFromStart = 0.0f;
    gsElapsedTimeCineDump = SECONDS_PER_TICK;
}
