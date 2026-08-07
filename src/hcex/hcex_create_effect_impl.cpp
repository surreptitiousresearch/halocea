/* hcex_create_effect_impl @0x823DE218 — instantiate a ws-engine entity that drives an HCEX effect/sfx.
 * Names the entity "hcex_effect<N>", builds its spawn transform from the (axis-converted, feet-scaled)
 * position, special-cases the plasma-grenade detonation cue, then creates the entity with SSL/farm
 * "suspend add" temporarily forced on. Releases the caller's effect-name string before returning.
 *
 * Halo->ws conversion: position (x,y,z)_halo -> (x, z, -y) scaled by 3.0480001. The entity/scene, sound,
 * string, and apCOUNTER profiler types are boundary types (see boundary header).
 *
 * deviation: the apCOUNTER profiling macro inlined around the body in the original (counter units 0 and 1:
 * timing capture, callsTotal increment, profiler Start/Stop) is represented as the two scope calls below. */

#include "../headers/hcex/hcex_create_effect_impl_boundary.h"

#define HALO_TO_WS_SCALE 3.0480001f

static int cstr_equals(const char *a, const char *b)
{
    while ( *a && *a == *b ) { ++a; ++b; }
    return *a == *b;
}

entENTITY *hcex_create_effect_impl(dsTSTRING_flat *hcexName, int obj_follow, hcex_float3 *pos)
{
    m3dV spawn_origin;
    entENTITY *entity;
    unsigned char ssl_suspend_saved;
    unsigned char farm_suspend_saved;

    cnt_hcex_create_sfx_enter();

    /* DEVIATION: `create_data` is scoped so its destructor runs where the binary runs it —
     * `bl animCREATE_DATA::~animCREATE_DATA` @0x823DE73C, BEFORE the dlFree @0x823DE754 that
     * releases the caller's name buffer. It used to be a function-scope local paired with
     * explicit entCREATE_DATA_ctor/_dtor shim calls, because this file's boundary header
     * restated entCREATE_DATA as a flat plain-C struct; it now uses the canonical RAII type
     * (odr_dup drain), whose ctor is the `bl entCREATE_DATA::entCREATE_DATA(void)` @0x823DE3EC. */
    {
    entCREATE_DATA create_data;

    sprintf_0(create_data.name, "hcex_effect%d", effectId++);

    spawn_origin.x = pos->x * HALO_TO_WS_SCALE;
    spawn_origin.y = pos->z * HALO_TO_WS_SCALE;
    spawn_origin.z = pos->y * -HALO_TO_WS_SCALE;
    m3dMATR_MakeLCS2WCS_VY(&create_data.matrInst, &spawn_origin, &m3dVUnitY);

    if ( cstr_equals(hcexName->pBuffer->str, "sfx_pg_detonation_start") )
    {
        snd_BUFFER_PARAMS_FULL params;
        dsSTRID armed_no_beeps;
        dsSTRID armed;
        dsSTRID *cue_id;
        snd_BUFFER *buffer;
        dsTSTRING_BUF_HEADER<char> *dbg_buffer;

        snd_BUFFER_PARAMS_ctor(&params);
        params.pos = spawn_origin;
        if ( obj_follow == -1 )
        {
            dsSTRID_ctor(&armed_no_beeps, "plasma_grenade_armed_no_beeps", 0);
            cue_id = &armed_no_beeps;
        }
        else
        {
            dsSTRID_ctor(&armed, "plasma_grenade_armed", 0);
            cue_id = &armed;
        }
        buffer = snd_System_GetBuffer(snd_System, cue_id, &params);
        if ( buffer )
        {
            snd_BUFFER_Play(buffer);
            snd_BUFFER_Release(buffer);
        }
        dbg_buffer = params.dbgInfo.pBuffer;
        if ( --dbg_buffer->refCount == 0 )
            dlFree(dbg_buffer);
    }

    /* suppress SSL/farm deferred-add while the effect entity is spawned, then restore */
    ssl_suspend_saved = (*gsSSL_SYSTEM_isSuspendAdd(gsSslSystem));
    farm_suspend_saved = (*farmSYSTEM_isSuspendAdd(farmSys));
    (*farmSYSTEM_isSuspendAdd(farmSys)) = 1;
    (*gsSSL_SYSTEM_isSuspendAdd(gsSslSystem)) = 1;
    entity = entCreate(gsScenePtr, hcexName->pBuffer->str, 0, &create_data, 0);
    (*farmSYSTEM_isSuspendAdd(farmSys)) = farm_suspend_saved;
    (*gsSSL_SYSTEM_isSuspendAdd(gsSslSystem)) = ssl_suspend_saved;

    cnt_hcex_create_sfx_exit();
    }   /* ~entCREATE_DATA (@0x823DE73C) */

    if ( --hcexName->pBuffer->refCount == 0 )
        dlFree(hcexName->pBuffer);

    return entity;
}
