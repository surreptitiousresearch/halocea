/* hcex_process_frame @0x823E8A40 — top-level per-frame entry point for the HCEX bridge (see
 * hcex_process_frame_boundary.h for the full feature rundown and a note on the scope of this
 * function's ~40 debug-console variable declarations). Snapshots cheat toggles, runs the legacy Blam
 * main-loop update, syncs first-person weapon state, dispatches legacy rendering, then runs a long
 * tail of debug-console-driven one-shot actions.
 *
 * deviation: the decompiler renders the apCOUNTER_TIME_CALL profiling macro (three nested/sequential
 * scopes: the whole-function "hcex/process_frame" RAII timer, the narrower "hcex_main_loop" scope,
 * and the narrower still "hcex_fp_sync" scope — each expanded inline as ~15 lines of per-thread
 * timestamp/callsTotal/myProfiler bookkeeping) as roughly 700 lines of register-juggling; collapsed
 * here into three enter/exit boundary-call pairs per this corpus's established convention (see
 * hcex_create_effect_impl.c, hcex_obj_sync_matrices.c). `hcex_shaders_fx_update`'s apparent argument
 * in the decompile (`hcex_shaders_fx_update(v125)`) is decompiler noise from that same profiling
 * macro's register reuse — confirmed void-void via its own already-reversed source
 * (src/hcex/hcex_shaders_fx_update.c) and disasm at this call site. The "dbgDumpMissingSfx" list-dump
 * loop's `v224 = 0x82000000` is unrelated decompiler garbage (a leftover register value never used as
 * anything but the loop index) and is omitted. Several function-local statics the decompiler models as
 * a shared `_S1_1` init-once guard byte (dbgToggleHaloWindow / dbgGodMode / dbgToggleStereo3D dsSTRID
 * keys, the mtlTypePrev cached string, the prevTime "always shoot" debounce timestamp) are reproduced
 * as ordinary C function-local `static` variables with their own init-once bool guards. */

#include "../headers/hcex/hcex_process_frame_boundary.h"
#include "../headers/material_type.h"

void hcex_process_frame(void)
{
    cheat.deathless_player = dbg_cheatDeathless.value;
    cheat.infinite_ammo = dbg_cheatInfAmmo.value || hcex_infinite_ammo;
    cheat.bump_possession = dbg_cheatBumpPossession.value;
    cheat.reflexive_damage_effects = dbg_cheatReflexiveDamageEffects.value;
    cheat.medusa = dbg_cheatMedusa.value;
    cheat.super_jump = dbg_cheatSuperJump.value;
    cheat.omnipotent = dbg_cheatOmnipotent.value;
    cheat.bottomless_clip = dbg_cheatBottomlessClip.value;

    if ( !hcex_call_frame || !main_coop_update() )
        return;

    osPIXBeginEvent("hcex_process_frame");
    cnt_hcex_process_frame_scope_enter();
    cnt_hcex_main_loop_enter();

    osPIXBeginEvent("hcex_main_loop");

    int is_legacy_render = (hcex_cur_render_mode == 0);
    if ( hcex_cur_render_mode )
    {
        osLOCK__Lock(&hcexHaloLogic, 0, 0);
    }
    else
    {
        gsRendPrepareRenderNextFrameSkip();
        vidLOCK_Lock(&vidLock, 0, 0);
    }

    if ( dbg_hcex_DropWeapons.value )
        cheat_all_weapons();
    if ( dbg_hcex_DropVehicles.value )
        cheat_all_vehicles();
    if ( dbg_hcex_DropCharacters.value )
        cheat_all_chars();
    if ( dbg_hcex_DropPowerups.value )
        cheat_all_powerups();
    if ( dbg_hcex_DropByName.value )
        cheat_by_model(dbg_hcex_ObjName.value.pBuffer->str);
    if ( dbg_hcex_DropVarByName.value )
        spawn_variant_near_player(dbg_hcex_VarName.value.pBuffer->str);

    osPIXBeginEvent("hcex_main_loop_start");
    main_loop_update();
    osPIXEndEvent();

    hcex_draw_string_flush();

    cnt_hcex_fp_sync_enter();
    osPIXBeginEvent("hcex_fp_sync");
    hcex_fp_sync();
    osPIXEndEvent();
    cnt_hcex_fp_sync_exit();

    if ( dbg_hcex_ReloadShaders.value )
        hcex_shaders_fx_update();

    if ( is_legacy_render )
    {
        vidLOCK_Unlock(&vidLock, 0, 0);
        jbmMANAGER_AddJob(&gsJobManager, 8u, &hcex_job_legacy_render);
    }
    else
    {
        if ( local_player_count() > 0 )
            hcex_render_ready = 1;
        if ( *(int *)&hcexHaloLogic > 0 )   /* lockDepth (head of osLOCK) */
            osLOCK_Unlock(&hcexHaloLogic, 0, 0);
    }

    main_loop_check_state();
    osPIXEndEvent();   /* "hcex_main_loop" */
    cnt_hcex_main_loop_exit();

    hcex_light_sync_all();
    hcex_create_effects_delayed();
    hcex_create_decals_delayed();

    if ( dbg_hcex_GenChpt.value )
    {
        if ( dbg_hcex_GenSafe.value )
            main_save_map_safe();
        else
            main_save_map_nonsafe();
    }

    osPIXBeginEvent("hcex_cheats");

    // deviation (2026-07-31): the raw decompile threads r3 between these two calls as a phantom
    // `active_camo_state` dataflow; the real functions are void cheat_teleport_to_camera(void) and
    // int cheat_active_camouflage(void) (independent toggles), so no return is captured or passed.
    if ( dbg_hcex_TeleportToCamera.value )
        cheat_teleport_to_camera();
    if ( dbg_hcex_ActiveCamo.value )
        cheat_active_camouflage();
    if ( dbg_hcex_DumpVariants.value )
        hcex_dump_variants();

    /* "legacy render mode" toggle: dbg_toggle_halo_window key-press OR the debug force-switch var. */
    static dsSTRID dbg_toggle_halo_window_key;
    static bool    dbg_toggle_halo_window_key_init;
    if ( !dbg_toggle_halo_window_key_init )
    {
        dsSTRID_ctor(&dbg_toggle_halo_window_key, "dbg_toggle_halo_window", 1);
        dbg_toggle_halo_window_key_init = true;
    }

    int legacy_mode_cfg = 0;
    if ( gCfg )
        apCONFIG_RetrieveInt(gCfg, "CONFIG", "LegacyMode", &legacy_mode_cfg, 0);

    gsiCONTEXT *dbg_context = gsINP_SYSTEM_GetDbgContext(gsSysInput);
    if ( gsiCONTEXT_IsJustOn(dbg_context, &dbg_toggle_halo_window_key) || dbg_hcex_SwitchMode.value )
    {
        legacy_mode_cfg = !legacy_mode_cfg;
        apCfgWriteInt("CONFIG", "LegacyMode", legacy_mode_cfg);
    }
    hcex_change_screen_mode(legacy_mode_cfg);

    /* god-mode toggle: dbg_god_mode key-press. */
    static dsSTRID dbg_god_mode_key;
    static bool    dbg_god_mode_key_init;
    if ( !dbg_god_mode_key_init )
    {
        dsSTRID_ctor(&dbg_god_mode_key, "dbg_god_mode", 1);
        dbg_god_mode_key_init = true;
    }

    dbg_context = gsINP_SYSTEM_GetDbgContext(gsSysInput);
    if ( gsiCONTEXT_IsJustOn(dbg_context, &dbg_god_mode_key) )
    {
        hcex_god_mode_toggle();

        dsTSTRING_flat message = { 0 };
        dsSPrintf(&message, "God mode %s", cheat.deathless_player ? "on" : "off");

        dsVECTOR_PARAM_LIST params = { 0 };
        dsSTRID text_key; text_key.id = "text";           // raw id (dsSTRID is not an aggregate now)
        dsVECTOR_PARAM_LIST_Set_tstring(&params, text_key, &message);
        int player_id = 0;
        dsSTRID player_id_key; player_id_key.id = "player_id";
        dsVECTOR_PARAM_LIST_Set_int(&params, player_id_key, &player_id);

        dsSTRID add_pickup_entry_event;
        dsSTRID_ctor(&add_pickup_entry_event, "addPickupEntry", 0);
        int event_id = dsEVENT_MGR_RegisterEvent(gEventMgr, add_pickup_entry_event);
        dsEVENT_MGR_SignalEvent(gEventMgr, event_id, static_cast<dsPARAM_LIST *>(&params)); // dsPARAM_LIST is the base
        dsVECTOR_dsPARAM_dtor(&params.storage);

        bool deathless = cheat.deathless_player != 0;
        dsTSTRING_flat dbg_var_name = { 0 };
        dsTSTRING_UnsafeInit(&dbg_var_name, "0HCEX/cheats/deathless player", -1, 0);
        dbgVAR_MANAGER_SetDbgVar_bool(dbgVAR_MANAGER_GetManager(), &dbg_var_name, &deathless);
        if ( --dbg_var_name.pBuffer->refCount == 0 )
            dlFree(dbg_var_name.pBuffer);

        if ( --message.pBuffer->refCount == 0 )
            dlFree(message.pBuffer);
    }

    /* stereo-3D toggle: dbg_toggle_stereo_3D key-press. */
    static dsSTRID dbg_toggle_stereo3d_key;
    static bool    dbg_toggle_stereo3d_key_init;
    if ( !dbg_toggle_stereo3d_key_init )
    {
        dsSTRID_ctor(&dbg_toggle_stereo3d_key, "dbg_toggle_stereo_3D", 1);
        dbg_toggle_stereo3d_key_init = true;
    }

    dbg_context = gsINP_SYSTEM_GetDbgContext(gsSysInput);
    if ( gsiCONTEXT_IsJustOn(dbg_context, &dbg_toggle_stereo3d_key) )
    {
        gsRENDER_SYSTEM_ToggleStereo3D(gsSysRender);
        vidDRIVER_SetCurConfigStereo3D(vidDriver, gsRENDER_SYSTEM_GetIsStereo3D(gsSysRender));
    }
    if ( gsRENDER_SYSTEM_GetIsStereo3D(gsSysRender) )
        gsRENDER_SYSTEM_SetStereo3DRatio(gsSysRender, director_is_first_person(0) ? 0.3f : 0.85f);

    hcex_update_skulls();
    hcex_set_pjl_effect(dbg_hcex_PjlEffect.value.pBuffer->str);

    /* material-effect-type re-resolve, only when the debug "MtlType" string actually changed. */
    static dsTSTRING_flat mtlTypePrev;
    static bool      mtlTypePrevInit;
    if ( !mtlTypePrevInit )
    {
        dsTSTRING_UnsafeInitEmpty(&mtlTypePrev);
        mtlTypePrevInit = true;
    }

    if ( strcmp(mtlTypePrev.pBuffer->str, dbg_hcex_MtlType.value.pBuffer->str) != 0 )
    {
        dsTSTRING_assign(&mtlTypePrev, &dbg_hcex_MtlType.value);

        hcex_material_effect_type = -1;
        for ( int i = 0; i < NUMBER_OF_MATERIAL_TYPES; ++i )
        {
            if ( strcmp(hcex_mtl_effects[i], dbg_hcex_MtlType.value.pBuffer->str) == 0 )
            {
                hcex_material_effect_type = i;
                break;
            }
        }
    }

    ai_globals_ai_active(dbg_hcex_AiIsActive.value);

    /* "always shoot" debounce: force the cheat off between period boundaries (dbg_hcex_always_shoot_period
     * seconds apart) so a held debug toggle doesn't refire every frame. */
    static float prevTime;
    static bool  prevTimeInit;
    float last_check_time;
    if ( prevTimeInit )
    {
        last_check_time = prevTime;
    }
    else
    {
        last_check_time = gsLvlTimer->timeLife;
        prevTime = gsLvlTimer->timeLife;
        prevTimeInit = true;
    }

    if ( (gsLvlTimer->timeLife - last_check_time) <= dbg_hcex_always_shoot_period.value )
    {
        hcex_always_shoot = 0;
    }
    else
    {
        prevTime = gsLvlTimer->timeLife;
        hcex_always_shoot = dbg_hcex_always_shoot.value;
    }

    debug_objects = dbg_hcex_DrawDebug.value;

    if ( dbg_hcex_EffectMoveTo.value && dbg_hcex_Effect.value.pBuffer->strLen )
        effect_move_to_next(dbg_hcex_Effect.value.pBuffer->str);

    hcex_fly_mode_speed_scale = dbg_hcex_flySpeedAccel.value;

    if ( dbg_hcexDumpUsedSfx.value )
    {
        apLogSetDumpRightNowMode(1);
        for ( int i = 0; i < hcexUsedSfx.nElem; ++i )
            _apDumpLog("used_sfx.log", "%s\n", hcexUsedSfx.pData[i].pBuffer->str);
        _apDumpLog("used_sfx.log", 0);
        apLogSetDumpRightNowMode(0);
    }
    if ( dbg_hcexClearUsedSfx.value )
        dsTSTRING_vector_Clear(&hcexUsedSfx);

    if ( dbg_hcexDumpUsedDecals.value )
    {
        apLogSetDumpRightNowMode(1);
        for ( int i = 0; i < hcexUsedDecals.nElem; ++i )
            _apDumpLog("used_decals.log", "%s\n", hcexUsedDecals.pData[i].pBuffer->str);
        _apDumpLog("used_decals.log", 0);
        apLogSetDumpRightNowMode(0);
    }
    if ( dbg_hcexClearUsedDecals.value )
        dsTSTRING_vector_Clear(&hcexUsedDecals);

    if ( dbg_hcexDumpMissingSfx.value )
    {
        apLogSetDumpRightNowMode(1);
        for ( int i = 0; i < hcexMissedSfx.nElem; ++i )
            _apDumpLog("missing_sfx.log", "%s\n", hcexMissedSfx.pData[i].pBuffer->str);
        _apDumpLog("missing_sfx.log", 0);
        apLogSetDumpRightNowMode(0);
    }

    if ( dbg_hcexDumpUsedCls.value )
    {
        apLogSetDumpRightNowMode(1);
        for ( int i = 0; i < hcexClsList.nElem; ++i )
            _apDumpLog("used_cls.log", "%s\n", hcexClsList.pData[i].pBuffer->str);
        _apDumpLog("used_cls.log", 0);
        apLogSetDumpRightNowMode(0);
    }
    if ( dbg_hcexClearUsedCls.value )
        dsTSTRING_vector_Clear(&hcexClsList);

    if ( dbg_hcexDumpUsedTex.value && hcexTexList.nElem )
    {
        apLogSetDumpRightNowMode(1);
        for ( int i = 0; i < hcexTexList.nElem; ++i )
            _apDumpLog("used_tex.log", "%s\n", hcexTexList.pData[i].pBuffer->str);
        _apDumpLog("used_tex.log", 0);
        apLogSetDumpRightNowMode(0);
    }
    if ( dbg_hcexClearUsedTex.value )
        dsTSTRING_vector_Clear(&hcexTexList);

    if ( dbg_hcexDumpScenery.value )
        hcex_dump_scenery();

    hcex_switch_cam_mode = dbg_hcex_SwitchCam.value;
    hcex_dbg_ignore_z = dbg_hcex_DrawIgnoreZ.value;
    hcex_debug_effect_names = dbg_hcex_DrawEffects.value;

    osPIXEndEvent();   /* "hcex_cheats" */
    osPIXEndEvent();   /* "hcex_process_frame" */
    cnt_hcex_process_frame_scope_exit();
}
