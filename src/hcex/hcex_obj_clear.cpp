/* hcex_obj_clear @0x823E0A58 — tear down all HCEX (Blam-bridge) render objects for a map change: stop the
 * render pipeline, terminate object/light/effect/model bridges, reset breakable surfaces, then walk the ws
 * scene destroying every "hcex_effect" instance and finally clear cinematic bridge objects.
 *
 * This is hcex-bridge code that calls into the ws-engine C++ layer; per the project's boundary conventions the
 * ws-engine container/scene/message methods (dsVECTOR::Clear, scnSCENE::FindNextInst, msgSYSTEM::SendMsg) are
 * declared as externs, not reversed. The hcex_* helpers below are the hcex-bridge subtree (declared here).
 * Deviation: the debug-only STRONG_ASSERT bounds check inside the light-destroy loop is elided. */

#include "../headers/animINST.h"

/* ws-engine boundary objects/methods (declared, not reversed) */
#include "../headers/hcex/hcex_light_vector.h"
extern "C" int strncmp(const char *a, const char *b, size_t n); // boundary — CRT
extern hcex_light_vector hcexLights;
extern void *hcexCreateEffects;
extern void *hcexModels;
extern struct scnSCENE *gsScenePtr;
extern struct msgSYSTEM msgSystem;
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

extern void dsVECTOR_HCEX_EFFECT_8__Clear(void *vector);
extern void dsVECTOR_dsPAIR_ulong_HCEX_MODEL_8__Clear(void *vector);
extern animINST *scnSCENE_FindNextInst(struct scnSCENE *scene, animINST *current);
extern void msgSYSTEM_SendMsg(struct msgSYSTEM *system, entENTITY *entity, int message, void *a, void *b);

/* hcex-bridge helpers (declared; reversed separately) */
extern unsigned char hcex_render_ready;
extern unsigned char hcex_obj_sync_matrices_in_progress;
extern void hcex_wait_jobs(void);
extern void hcex_obj_term_all(void);
extern "C" void hcex_destroy_light(int lightId); /* DEVIATION: int lightId, not unsigned long -- def src/hcex/hcex_destroy_light.cpp + prologue stw r3 @0x823DE13C */
extern void hcex_fp_models_term(void);
extern void hcex_term_breakable_surfaces(int flag);
extern void hcex_destroy_dead_intances(void);
extern void hcex_init_breakable_surfaces(void);
extern void hcex_cine_obj_clear(void);

// DEVIATION: `extern "C"`. The binary exports this as the FLAT symbol `hcex_obj_clear` (@0x823E0A58);
// compiled as C++ without it the definition mangles to `?hcex_obj_clear@@YA...` and nothing links
// against it. 209 of the 219 flat-defining hcex TUs already do this -- these ten did not.
extern "C" void hcex_obj_clear(void)
{
    hcex_render_ready = 0;
    hcex_wait_jobs();
    hcex_obj_sync_matrices_in_progress = 0;
    hcex_obj_term_all();

    while ( hcexLights.nElem )
        hcex_destroy_light(hcexLights.pData->key);

    hcex_fp_models_term();
    dsVECTOR_HCEX_EFFECT_8__Clear(&hcexCreateEffects);
    dsVECTOR_dsPAIR_ulong_HCEX_MODEL_8__Clear(&hcexModels);
    hcex_term_breakable_surfaces(0);
    hcex_destroy_dead_intances();
    hcex_init_breakable_surfaces();

    /* destroy every scene instance named "hcex_effect" */
    animINST *instance = 0;
    animINST *next;
    do
    {
        next = scnSCENE_FindNextInst(gsScenePtr, instance);
        if ( instance && instance->pEnt )
        {
            const char *name = instance->name;
            if ( !name )
                name = empty_string;
            if ( !strncmp(name, "hcex_effect", 0xB) )
                msgSYSTEM_SendMsg(&msgSystem, instance->pEnt, 2, 0, 0);
        }
        instance = next;
    }
    while ( next );

    hcex_cine_obj_clear();
}
