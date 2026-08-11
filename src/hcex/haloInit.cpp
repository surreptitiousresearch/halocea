/* haloInit @0x823EA248 — wires the legacy Blam engine into the ws-engine (see CLAUDE.md's Blam
 * Bridge table). Registers scenery entity handlers, the Halo script-callable functions
 * (SetHaloCamActive/HcexTeleportToCam/HcexMakeAutosave/IsHaloMode), the "entity"/"ent_light" and
 * "prop_ent"/"halo_obj" brand hierarchies, then — only when running in Halo mode (`load_as_halo`)
 * — swaps in the HCEX particle system and custom scenery instance-manager handler, spawns the
 * HALO_MNG interactive actor (which hosts the Blam main loop, ticked every frame), wires the
 * render hook (hcexRenderInit) and the ingame-text debug toggle, brings up haloENGINE_CONTROL,
 * registers the per-frame preprocess callback, applies split-screen mode, and finally starts the
 * Blam main loop (main_loop_init1), optionally forcing deathless-player film playback.
 *
 * The vtable-slot-0x34 call after CreateIA (disasm `lwz r10, 0x34(r11)`) is SetName —
 * DB-verified via types_members HALO_MNG_vtbl::SetName @0x34; dispatched as the typed
 * iaIACTOR::SetName virtual (declared in iaIACTOR.h). */

#include "../headers/hcex/haloInit_boundary.h"

#include <new>
#include "headers/game_time_constants.h"

int haloInit()
{
    dsTSTRING<char> t1, t2;

    t1.pBuffer = nullptr;
    t1.UnsafeInit("entity", -1, 0);
    t2.pBuffer = nullptr;
    t2.UnsafeInit("ent_light", -1, 0);
    dscBRAND *entLightBrand = iaFamily->RegisterBrand<entLIGHT_DESC>(t2, t1);
    if (entLightBrand)
        entLightBrand[1].__vftable = (dscBRAND_vtbl *)dsMakeFunc<void, entLIGHT>;

    dsTSTRING<char> t3, t4;
    t3.pBuffer = nullptr;
    t3.UnsafeInit("prop_ent", -1, 0);
    t4.pBuffer = nullptr;
    t4.UnsafeInit("halo_obj", -1, 0);
    pPropFamily->RegisterAbstractBrand<propHALO_OBJ>(t4, t3);

    gsElapsedTimeMin = SECONDS_PER_TICK;

    entManager.AddHandler((char *)"halo_model", &hcexHaloModel);
    entManager.AddHandler((char *)"halo_object", &hcexHaloObject);

    sslCLASS_REF halo = gsSslSystem->GetGlobalObj().GetClass();
    halo.AddCbFunc("SetHaloCamActive(isAct : bool)", cbSetHaloCamActive, empty_string, nullptr);

    sslCLASS_REF halo2 = gsSslSystem->GetGlobalObj().GetClass();
    halo2.AddCbFunc("HcexTeleportToCam()", cbHcexTeleportToCam, empty_string, nullptr);

    sslCLASS_REF halo3 = gsSslSystem->GetGlobalObj().GetClass();
    halo3.AddCbFunc("HcexMakeAutosave()", cbMakeAutosave, empty_string, nullptr);

    sslCLASS_REF halo4 = gsSslSystem->GetGlobalObj().GetClass();
    halo4.AddCbFunc("IsHaloMode() : bool", cbIsHaloMode, empty_string, nullptr);

    hcex_library_init();
    hcex_cine_init();
    InputQueueInit();
    haloEngineCtrlInit();
    hcex_effect_names_parse();

    if (load_as_halo)
    {
        if (gsSysParticle)
            gsSysParticle->__vftable->dtr_gsPARTICLE_SYS(gsSysParticle, 1);
        hcexPARTICLE_SYS *newParticleSys = (hcexPARTICLE_SYS *)operator new(4u, "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 0x106E);
        if (newParticleSys)
        {
            new (newParticleSys) hcexPARTICLE_SYS();
            gsSysParticle = newParticleSys;
        }
        else
        {
            gsSysParticle = nullptr;
        }

        if (!IGNORE_STRONG_ASSERT && instManager)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("!instManager",
                                        "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 4208, empty_string);

        hcexHALO_CUSTOM_OBJ_LIST *customObjList = (hcexHALO_CUSTOM_OBJ_LIST *)operator new(
            0x18u, "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 0x1071);
        if (customObjList)
        {
            new (customObjList) hcexHALO_CUSTOM_OBJ_LIST();
            instManager = customObjList;
        }
        else
        {
            instManager = nullptr;
        }

        UI_EXT_RENDER = hcex_render_hud;

        dsTSTRING<char> t5, t6;
        t5.pBuffer = nullptr;
        t5.UnsafeInit("iactor", -1, 0);
        t6.pBuffer = nullptr;
        t6.UnsafeInit("halo_mng", -1, 0);
        dscBRAND *haloMngBrand = iaFamily->RegisterBrand<HALO_MNG_DESC>(t6, t5);
        if (haloMngBrand)
            haloMngBrand[1].__vftable = (dscBRAND_vtbl *)dsMakeFunc<void, HALO_MNG>;

        dsTSTRING<char> t7;
        t7.pBuffer = nullptr;
        t7.UnsafeInit("halo_mng", -1, 0);
        HALO_MNG *actor = iaFamily->CreateIA<HALO_MNG>(t7);

        dsTSTRING<char> t8;
        t8.pBuffer = nullptr;
        t8.UnsafeInit("halo_mng", -1, 0);
        actor->SetName(&t8); // typed virtual (DB HALO_MNG_vtbl::SetName @0x34)

        actor->SetPriority(0x10u);
        actor->Register(nullptr);

        hcexRenderInit();

        dsTSTRING<char> t9;
        t9.pBuffer = nullptr;
        t9.UnsafeInit("ui/ingame text/enable", -1, 0);
        dbgVAR_MANAGER *dbgMgr = dbgVAR_MANAGER_GetManager();
        dbgVAR *ingameText = dbgVAR_MANAGER_FindFirst(dbgMgr, &t9);
        if (ingameText && ingameText->typeId == 1)
        {
            ((dbgVAR_bool *)ingameText)->value = true;
            dbgVAR_MANAGER_SendVarUpdate(dbgMgr, ingameText);
        }

        debug_no_drawing = 1;
        vidLOCK_Lock(vidLock, nullptr, 0);
        haloInitC();
        vidLOCK_Unlock(vidLock, nullptr, 0);

        onPreProcessFrame.PushBack(hcex_preproc_frame);

        if (hcex_split_screen)
        {
            ENGINE_CONTROL_SetSplitScreen(gsEngineCtrl, 1);
            player_spawn_count = 2;
        }
        else
        {
            ENGINE_CONTROL_SetSplitScreen(gsEngineCtrl, 0);
        }

        int demoPlay = 0;
        bool demoPlayFound = gCfg->GetValue<int>("Debug.HcexDemoplay", &demoPlay);
        if (!demoPlayFound)
            demoPlay = 0;
        if (!gCfg->GetValue<int>("Debug.HcexDemorec", &hcex_allow_saved_film_recoring))
            hcex_allow_saved_film_recoring = 0;

        int filmToken = main_loop_init1();

        if (demoPlay)
        {
            cheat.deathless_player = 1;
            main_set_game_connection_to_film_playback(filmToken);
        }

        haloENGINE_CONTROL_InitSettings(haloEngineCtrl);
    }

    return 1;
}
