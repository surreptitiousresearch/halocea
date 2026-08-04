#include "../headers/ws/ssl/sslOBJ_REF.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../headers/hcex/hcex_render_hud_boundary.h" // hcex_off_cine_screen_effect

// cbSetHaloCineScreenEffectActive @ 0x823B4BC0 — SSL callback
// "SetHaloCineScreenEffectActive(isOn : bool)". Toggle the cinematic letterbox/screen effect; the
// HCEX suppression flag is the logical negation of the requested "on" state.
extern "C" void cbSetHaloCineScreenEffectActive(sslOBJ_REF self, int argc, dsDATA *argv,
                                                dsDATA *retVal, sslOBJ_REF caller)
{
    (void)retVal;
    if (argc >= 1)
    {
        bool isOn;
        argv->GetValue<bool>(isOn, DSD_CONV_RETRIEVE);
        hcex_off_cine_screen_effect = !isOn;
    }
}
