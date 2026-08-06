/* haloTerm @0x823CF1B8 — tears down the HCEX/Blam bridge installed by haloInit: releases the
 * custom instance-manager handler, the halo engine-control object, the input queue, and the
 * loading-screen registration, then (only for the standalone/tool build path, when
 * `load_as_halo` is set) shuts the Blam main loop down via main_loop_term.
 *
 * DEVIATION — instManager->dtr_instMANAGER_HANDLER: disasm confirms the call loads its function
 * pointer from vftable+4 (not +0) and passes a second `freeMemory=1` argument; see the
 * DEVIATION note in instMANAGER_HANDLER.h (this pass corrected that header's slot order to
 * match). */

#include "../headers/ws/inst/instMANAGER_HANDLER.h"

extern instMANAGER_HANDLER *instManager;
extern int                  load_as_halo;

extern void haloEngineCtrlTerm();
extern void InputQueueTerm();
extern void hcex_loading_screen_term();
extern "C" void main_loop_term(void); /* attested void */

int haloTerm()
{
    if (instManager)
        instManager->__vftable->dtr_instMANAGER_HANDLER(instManager, 1);
    instManager = nullptr;

    haloEngineCtrlTerm();
    InputQueueTerm();
    hcex_loading_screen_term();

    if (load_as_halo)
        main_loop_term();

    return 1;
}
