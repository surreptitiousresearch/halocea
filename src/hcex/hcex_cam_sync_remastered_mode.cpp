/* hcex_cam_sync_remastered_mode @0x823D1458 — per-frame HCEX bridge step for the remastered renderer: refresh
 * the Blam render cameras, run the no-render first-person weapon update, push the cameras into the Blam
 * rasterizer, and flush any delayed particle create/destroy on the ws-engine particle system. Then, when
 * remastered rendering is active (hcex_cur_render_mode != 0), request a normal next-frame render prepare
 * unless the anitec cinematic camera owns the frame; when it is inactive, request a skipped prepare and, if
 * the prepare-render job wants to run from ProcessFrame, clear its initialPrepare flag and queue it on the job
 * manager.
 *
 * DEVIATION: gsSysParticle->StopDelayedCDT is a virtual dispatch (vtable +0x24); it is written here as a named
 * boundary call. gsSysAnitec->pObjCam (@ +0xEC) and gsJobPrepareRender.initialPrepare (@ +0xB840) /
 * .doPrepareFromProcessFrame (@ +0xB844) are accessed by DB-confirmed raw offsets — those ws-engine types are
 * boundaries and not modeled in full. AddJob's thread-mask argument (2) is an apSTATE_T<unsigned long> passed
 * by value. */

extern struct gsPARTICLE_SYS *gsSysParticle;
extern struct gsANITEC_SYS   *gsSysAnitec;
extern struct jbmMANAGER      gsJobManager;
extern char gsJobPrepareRender[];   /* ws-engine gsREND_JOB_PREPARE_RENDER (jbmJOB_EX base) — boundary; reached by DB offset */
extern int  hcex_cur_render_mode;

extern "C" void hcex_update_halo_render(void);
extern "C" void first_person_weapon_update_no_rend(void);
extern "C" void hcex_cam_sync(void);
extern void gsPARTICLE_SYS_StopDelayedCDT(struct gsPARTICLE_SYS *system, int mode);  /* virtual, vtbl+0x24 */
extern void gsRendPrepareRenderNextFrame(void);
extern void gsRendPrepareRenderNextFrameSkip(void);
extern void jbmMANAGER_AddJob(struct jbmMANAGER *manager, unsigned long thread_mask, void *job);

/* DB-confirmed gsREND_JOB_PREPARE_RENDER field offsets */
#define GS_JOB_PREPARE_INITIAL_PREPARE          47168 /* 0xB840 .initialPrepare */
#define GS_JOB_PREPARE_DO_PREPARE_FROM_PROCESS  47172 /* 0xB844 .doPrepareFromProcessFrame */

extern "C" void hcex_cam_sync_remastered_mode(void)
{
    hcex_update_halo_render();
    first_person_weapon_update_no_rend();
    hcex_cam_sync();
    gsPARTICLE_SYS_StopDelayedCDT(gsSysParticle, 2);

    if ( hcex_cur_render_mode )
    {
        if ( !*(void **)((char *)gsSysAnitec + 236) )   /* gsANITEC_SYS.pObjCam @ +0xEC */
            gsRendPrepareRenderNextFrame();
    }
    else
    {
        gsRendPrepareRenderNextFrameSkip();
        if ( *(int *)(gsJobPrepareRender + GS_JOB_PREPARE_DO_PREPARE_FROM_PROCESS) )
        {
            *(int *)(gsJobPrepareRender + GS_JOB_PREPARE_INITIAL_PREPARE) = 0;
            jbmMANAGER_AddJob(&gsJobManager, 2, gsJobPrepareRender);
        }
    }
}
