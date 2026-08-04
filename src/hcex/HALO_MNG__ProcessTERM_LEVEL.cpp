#include "../headers/hcex/HALO_MNG.h"
#include "../headers/hcex/HALO_MNG_boundary.h"
#include "../headers/ws/inst/instMANAGER_HANDLER.h" // complete type for instManager+1 pointer arithmetic

// 0x823DE98C — chain to the base, then tear down per-level HCEX state: clear the pending
// instance-creation-data queue (instManager's second sub-vector), stop treating this as an active
// call-frame, terminate first-person models / breakable surfaces / dead & live scene instances /
// cinematics, and tell the custom sound system its level is ending.
void HALO_MNG::ProcessTERM_LEVEL()
{
    iaIACTOR::ProcessTERM_LEVEL();

    // DEVIATION: instManager is a global instMANAGER_HANDLER* (opaque, not reversed in this
    // batch); the binary reinterprets `&instManager[1]` as a
    // dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8>* to clear a sub-vector embedded past
    // the handler's own fields. Reproduced verbatim (not reinterpreting instMANAGER_HANDLER's
    // layout, which is out of scope here).
    reinterpret_cast<dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8> *>(instManager + 1)->Clear();

    hcex_call_frame = false;
    hcex_fp_models_term();
    hcex_term_breakable_surfaces(1);
    hcex_destroy_dead_intances();
    hcex_obj_term_instances();
    hcex_cine_term_level();
    snd::SystemCustom->TermLevel();
}
