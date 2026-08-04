#include "../headers/ws/ssl/sslOBJ_REF.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/hcex/hcex_process_frame_boundary.h" // cheat_teleport_to_camera

// cbHcexTeleportToCam @ 0x823CEC40 — SSL callback "HcexTeleportToCam()". Teleport the player to the
// current debug/cinematic camera position.
extern "C" void cbHcexTeleportToCam(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal,
                                    sslOBJ_REF caller)
{
    (void)argc;
    (void)argv;
    (void)retVal;
    // Boundary function takes no arguments; the SSL argv quadruple the decompiler forwards is
    // leftover callback registers.
    cheat_teleport_to_camera();
}
