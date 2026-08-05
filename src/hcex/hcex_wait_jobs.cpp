/* hcex_wait_jobs @0x823D1328 — drain the ws-engine job manager so the Blam bridge can mutate shared state
 * safely: release the halo-logic lock if held, clear the "render in flight" app-state bit, wait for all job
 * threads to finish, then restore the bit.
 *
 * hcex-bridge code calling into the ws-engine job/lock layer; those primitives are declared as boundary externs.
 * Deviation: the debug-only STRONG_ASSERT (osGetCurThreadProcessor()==0) is elided. */

#include "../headers/ws/os/osLOCK.h"
extern struct osLOCK hcexHaloLogic;
extern struct jbmMANAGER gsJobManager;
extern unsigned int gsAppState2;
extern void osLOCK_Unlock(struct osLOCK *lock, void *site, int flag);
extern void jbmMANAGER_WaitThreadsDone(struct jbmMANAGER *manager, void *state);

void hcex_wait_jobs(void)
{
    if ( hcexHaloLogic.lockDepth > 0 )   /* DEVIATION: was *(int*)& head-pun; binary reads osLOCK.lockDepth (+0x24) */
        osLOCK_Unlock(&hcexHaloLogic, 0, 0);

    gsAppState2 &= ~0x20000u;
    jbmMANAGER_WaitThreadsDone(&gsJobManager, (void *)0xFE);
    gsAppState2 |= 0x20000u;
}
