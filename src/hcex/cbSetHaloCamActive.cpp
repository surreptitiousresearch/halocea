#include "../headers/ws/ssl/sslOBJ_REF.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../headers/hcex/hcex_set_cam_boundary.h" // hcex_off_cam

// cbSetHaloCamActive @ 0x823D5208 — SSL callback "SetHaloCamActive(isAct : bool)". When the camera
// is made active (isAct == true) the HCEX camera-override flag is cleared, and vice versa.
// self/caller are passed by value (their refs are released by the by-value parameter destructors on
// return, matching the decompiler's trailing ~sslOBJ_REF calls).
extern "C" void cbSetHaloCamActive(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal,
                                   sslOBJ_REF caller)
{
    (void)retVal;
    if (argc >= 1)
    {
        bool isActive;
        argv->GetValue<bool>(isActive, DSD_CONV_RETRIEVE);
        // Decompiler renders this as (_cntlzw(v7) & 0x20) != 0, i.e. the logical negation of isActive.
        hcex_off_cam = !isActive;
    }
}
