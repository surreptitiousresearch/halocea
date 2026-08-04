/* input_set_gamepad_vibrater_state @0x823CA3A0 — drive controller rumble for a given controller port. Scans
 * every connected ws-engine input device, and for the one bound to `ctrl_idx` sets its two rumble motors from
 * the 16-bit `left`/`right` magnitudes, scaled by the dbg_hcex_inp_vibration_scale console knob and
 * normalised against 65535. Vibration is forced off while the game clock is paused, when the hcex_off_vibration
 * override is set, and — in split-screen co-op — for every controller except the designated local player's
 * (which is remapped to port 0).
 *
 * DEVIATION vs decompiler: the two device virtuals are now dispatched through the typed
 * inpDEVICE_vtbl (GetInstId@0x38, SetVibration@0x2C = (this, float left, float right, int enable)).
 * The decompiler's earlier 6-argument untangle ("raw motor ints carrying leftover vtable/device
 * registers") was an ABI misread: on PPC the float args ride f1/f2 and the enable int rides the
 * 4th GPR slot (r6=1 in the disasm), so with the DB prototype the phantom int args vanish.
 * game_time_get_paused is a Blam boundary. */

#include <stdint.h>
#include "../headers/hcex/hcex_dbgvar_boundary.h"   /* dbgVAR_float, dbg_hcex_inp_vibration_scale */
#include "../headers/ws/inp/inpDEVICE_GAMEPAD_XENON.h" /* typed inpDEVICE vtbl + gamepad.bConnected */

typedef struct gsINP_SYSTEM gsINP_SYSTEM;
typedef struct inpINPUT inpINPUT;

extern gsINP_SYSTEM *gsSysInput;
extern inpINPUT  *gsINP_SYSTEM_GetInput(gsINP_SYSTEM *self);
extern int        inpINPUT_GetDeviceCount(inpINPUT *self);
extern inpDEVICE *inpINPUT_GetDevice(inpINPUT *self, int id);

extern uint8_t game_time_get_paused(void);
extern int           hcex_coop_local_player_index;
extern unsigned char hcex_off_vibration;

extern "C" void input_set_gamepad_vibrater_state(int ctrl_idx, unsigned __int16 left, unsigned __int16 right)
{
    float scale = dbg_hcex_inp_vibration_scale.value;
    if ( game_time_get_paused() )
        scale = 0.0f;

    if ( hcex_coop_local_player_index >= 0 )
    {
        if ( ctrl_idx != hcex_coop_local_player_index )
            return;
        ctrl_idx = 0;
    }

    for ( int i = 0; i < inpINPUT_GetDeviceCount(gsINP_SYSTEM_GetInput(gsSysInput)); ++i )
    {
        inpDEVICE *device = inpINPUT_GetDevice(gsINP_SYSTEM_GetInput(gsSysInput), i);

        /* typed: was raw device+0x11C4 (inpDEVICE_GAMEPAD_XENON::gamepad.bConnected) */
        if ( ((inpDEVICE_GAMEPAD_XENON *)device)->gamepad.bConnected == 0 )
            continue;

        if ( device->__vftable->GetInstId(device) != ctrl_idx )
            continue;

        if ( hcex_off_vibration )
        {
            device->__vftable->SetVibration(device, 0.0f, 0.0f, 1);
        }
        else
        {
            float left_scale  = (float)((float)(scale * (float)(2 * left))  / 65535.0f);
            float right_scale = (float)((float)(scale * (float)(2 * right)) / 65535.0f);
            device->__vftable->SetVibration(device, left_scale, right_scale, 1);
        }
    }
}
