/* hcex_cine_update_anim @0x823BA3A0 — per-frame HCEX debug cinematic-actor position dump: while a debug
 * cine capture is active and dbg_hcex_DumpCine is on, for every tracked HCEX_OBJ with a currently
 * playing animation, record its animation-instance world matrix into the per-actor debug map
 * (_dbgCineInfo.mapActors), lazily creating each actor's entry (and its
 * <_dbgCineInfo.path>\cine_log_pos_<key>.txt output file) on first use. The map is keyed by the
 * animation template name by default, or by the object's scenario name when it has one (both the key
 * and the dump-file name switch together). Each entry only dumps once per frame
 * (DBG_CINE_ACTOR_INFO::wasUpdatedCurFrame), so a final pass over the whole map resets that flag for
 * the next frame before the capture's frame counter and elapsed time advance.
 *
 * DEVIATION: the decompiler shows the name-based key reassignment as `v47.id = *v16` with `v16` never
 * assigned in the visible pseudocode — a register-tracking artifact of the freshly-constructed
 * name-based dsSTRID (`v56`) being copied into the key used below. Reconstructed as the evident intent:
 * the key switches to the object's scenario name whenever one is present, exactly mirroring how the
 * dump-file path switches from the template-name form to the scenario-name form in the same branch. */

#include "../headers/hcex/hcex_cine_tick_boundary.h"

extern void animINST_GetMatrL2W(animINST *self, void *matrL2W_out); /* animINST::GetMatrL2W(m3dMATR*) */

void hcex_cine_update_anim(void)
{
    if ( !(hcex_is_cine_mode() && dbg_hcex_DumpCine.value && _dbgCineInfo.isInited) )
        return;

    int idx = 1;
    for ( HCEX_OBJ *obj = hcex_obj_get_by_idx(0); obj; obj = hcex_obj_get_by_idx(idx++) )
    {
        animINST *inst = obj->pInst;

        int frameCur, frameCount;
        char *animName = 0;
        if ( !hcex_obj_get_anim_info(obj->id, &frameCur, &frameCount, &animName) )
            continue;

        const char *tplName = inst->pTpl->name;

        dsSTRID dbgKey;
        dsSTRID_ctor(&dbgKey, tplName, 0);

        dsTSTRING_flat dumpPath;
        dumpPath.pBuffer = 0;
        dsTSTRING_concat_cstr(&dumpPath, &_dbgCineInfo.path, "\\cine_log_pos_");
        dsTSTRING_Insert(&dumpPath, dumpPath.pBuffer->strLen, tplName ? tplName : empty_string, -1);
        dsTSTRING_Insert(&dumpPath, dumpPath.pBuffer->strLen, ".txt", -1);

        const char *objName = hcex_obj_get_name(obj->id);
        if ( objName )
        {
            dsSTRID_ctor(&dbgKey, objName, 0);

            if ( --dumpPath.pBuffer->refCount == 0 )
                dlFree(dumpPath.pBuffer);
            dumpPath.pBuffer = 0;
            dsTSTRING_concat_cstr(&dumpPath, &_dbgCineInfo.path, "\\cine_log_pos_");
            dsTSTRING_Insert(&dumpPath, dumpPath.pBuffer->strLen, objName, -1);
            dsTSTRING_Insert(&dumpPath, dumpPath.pBuffer->strLen, ".txt", -1);
        }

        char matrL2W[64]; /* m3dMATR (64 bytes, opaque here — passed straight through) */
        animINST_GetMatrL2W(inst, matrL2W);

        DBG_CINE_ACTOR_MAP_ITER it = DBG_CINE_INFO_mapActors_Find(&_dbgCineInfo.mapActors, &dbgKey);
        if ( !it.item )
        {
            DBG_CINE_ACTOR_INFO newEntry;
            DBG_CINE_ACTOR_INFO_ctor(&newEntry, &dumpPath, _dbgCineInfo.timeFromStart);
            it = DBG_CINE_INFO_mapActors_FindInsert(&_dbgCineInfo.mapActors, &dbgKey, &newEntry);

            if ( --newEntry.filePath.pBuffer->refCount == 0 )
                dlFree(newEntry.filePath.pBuffer);
        }

        DBG_CINE_ACTOR_INFO *actorInfo = DBG_CINE_INFO_mapActors_Deref(it);
        if ( !actorInfo->wasUpdatedCurFrame )
        {
            DBG_CINE_ACTOR_INFO_DumpMatrix(actorInfo, matrL2W);
            actorInfo->wasUpdatedCurFrame = 1;
        }

        if ( --dumpPath.pBuffer->refCount == 0 )
            dlFree(dumpPath.pBuffer);
    }

    for ( DBG_CINE_ACTOR_MAP_ITER walk = DBG_CINE_INFO_mapActors_Begin(&_dbgCineInfo.mapActors);
          walk.item;
          walk = DBG_CINE_INFO_mapActors_Next(walk) )
    {
        DBG_CINE_INFO_mapActors_Deref(walk)->wasUpdatedCurFrame = 0;
    }

    if ( _dbgCineInfo.isInited )
    {
        ++_dbgCineInfo.frame;
        _dbgCineInfo.timeFromStart = _dbgCineInfo.timeFromStart + gsElapsedTime;
    }
}
