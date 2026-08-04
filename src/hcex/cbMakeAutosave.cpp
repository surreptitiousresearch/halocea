#include "../headers/ws/ssl/sslOBJ_REF.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h" // load_as_halo
#include "../headers/hcex/hcex_process_frame_boundary.h"                // main_save_map_nonsafe

// cbMakeAutosave @ 0x823CEBE8 — SSL callback "HcexMakeAutosave()". Trigger a non-safe autosave of
// the current map, but only while the legacy Blam game is loaded.
extern "C" void cbMakeAutosave(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal,
                               sslOBJ_REF caller)
{
    (void)argc;
    (void)argv;
    (void)retVal;
    // The decompiler shows the SSL argv quadruple forwarded into main_save_map_nonsafe; the real
    // boundary function takes no arguments (they are leftover callback registers), so it is called
    // with no arguments here.
    if (load_as_halo)
        main_save_map_nonsafe();
}
