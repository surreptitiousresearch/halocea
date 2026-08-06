/* hcex_update_gamepad_x360 @0x823CA5A8 — per-frame HCEX bridge poll of the ws-engine input
 * subsystem: clears the shadow gamepad state, then walks every attached inpDEVICE and, for each
 * one bound to controller port 0 or 1 (remapped through hcex_coop_local_player_index in
 * split-screen coop, same convention as input_set_gamepad_vibrater_state), copies its raw
 * thumbstick/button state into the HCEX shadow arrays (hcex_sThumbLX/LY/RX/RY,
 * hcex_gamepad_buttons). In coop when the local player owns port 0, it finally replays the
 * previously-received remote player's raw input (hcex_coop_u1_inp_remote) into port 1 via
 * hcex_raw_input_set, since the remote player has no locally-attached inpDEVICE of its own.
 *
 * gsINP_SYSTEM / inpINPUT / inpDEVICE are ws-engine (Saber) input boundaries. clear_hcex_input_x360
 * is a pre-existing hcex helper not reversed in this batch.
 *
 * DEVIATION (typed subtype resolved): the decompiler renders three per-device fields as
 * pointer arithmetic one whole inpDEVICE past the object. These are the inpDEVICE_GAMEPAD_XENON
 * subtype (DB types_members): gamepad.bConnected @+0x11C4 and the raw XInput thumbsticks
 * gamepad.base.sThumbLX/LY @+0x1158, sThumbRX/RY @+0x115C, read as typed members.
 * `(_cntlzw(x) & 0x20) == 0` is the codebase's established "x != 0" boolean idiom.
 * GetInstId is dispatched through the typed inpDEVICE_vtbl (slot 0x38). */

#include <stdint.h>
#include "../headers/hcex/haloRAW_INPUT_X360.h"
#include "../headers/ws/inp/inpDEVICE_GAMEPAD_XENON.h" /* typed inpDEVICE vtbl + gamepad state */

typedef struct gsINP_SYSTEM gsINP_SYSTEM;
typedef struct inpINPUT inpINPUT;

extern gsINP_SYSTEM *gsSysInput;
extern inpINPUT  *gsINP_SYSTEM_GetInput(gsINP_SYSTEM *self);
extern int        inpINPUT_GetDeviceCount(inpINPUT *self);
extern inpDEVICE *inpINPUT_GetDevice(inpINPUT *self, int id);
extern float      inpDEVICE_GetKeyValue(inpDEVICE *self, int key_index);   /* non-virtual */

extern "C" void clear_hcex_input_x360(void);
extern void hcex_raw_input_set(int gamepadId, const haloRAW_INPUT_X360 *inp);

extern "C" int    hcex_coop_local_player_index;
extern haloRAW_INPUT_X360 hcex_coop_u1_inp_remote;

extern "C" char    hcex_gamepad_buttons[4][16];
extern "C" int16_t hcex_sThumbLX[4];
extern "C" int16_t hcex_sThumbLY[4];
extern "C" int16_t hcex_sThumbRX[4];
extern "C" int16_t hcex_sThumbRY[4];

void hcex_update_gamepad_x360(void)
{
    clear_hcex_input_x360();

    inpINPUT *input = gsINP_SYSTEM_GetInput(gsSysInput);
    int device_count = inpINPUT_GetDeviceCount(input);

    for ( int i = 0; i < device_count; ++i )
    {
        inpDEVICE *device = inpINPUT_GetDevice(gsINP_SYSTEM_GetInput(gsSysInput), i);

        inpDEVICE_GAMEPAD_XENON *pad = (inpDEVICE_GAMEPAD_XENON *)device;

        /* typed (DB inpDEVICE_GAMEPAD_XENON::gamepad.bConnected; was raw device+0x11C4) */
        if ( pad->gamepad.bConnected == 0 )
            continue;

        unsigned int port = (unsigned int)device->__vftable->GetInstId(device); /* typed vtbl slot 0x38 */

        if ( port > 1 )
            continue;

        if ( hcex_coop_local_player_index >= 0 )
        {
            /* coop active: port-1 devices are ignored here — the remote player's input
             * arrives over the network (hcex_coop_u1_inp_remote), not from a local device.
             * Only the local player's device (port 0) is remapped to the coop-assigned slot. */
            if ( port != 0 )
                continue;
            port = hcex_coop_local_player_index;
        }
        /* else: no coop remap active — use the raw port as-is */

        /* typed (gamepad.base.sThumbLX/LY/RX/RY; was two packed big-endian DWORD reads
           @+0x1158/+0x115C -- identical field pairs on this big-endian target) */
        hcex_sThumbLX[port] = pad->gamepad.base.sThumbLX;
        hcex_sThumbLY[port] = pad->gamepad.base.sThumbLY;
        hcex_sThumbRX[port] = pad->gamepad.base.sThumbRX;
        hcex_sThumbRY[port] = pad->gamepad.base.sThumbRY;

        hcex_gamepad_buttons[port][0]  = inpDEVICE_GetKeyValue(device, 8)  > 0.000001f;
        hcex_gamepad_buttons[port][1]  = inpDEVICE_GetKeyValue(device, 9)  > 0.000001f;
        hcex_gamepad_buttons[port][2]  = inpDEVICE_GetKeyValue(device, 10) > 0.000001f;
        hcex_gamepad_buttons[port][3]  = inpDEVICE_GetKeyValue(device, 11) > 0.000001f;
        hcex_gamepad_buttons[port][6]  = inpDEVICE_GetKeyValue(device, 14) > 0.000001f;
        hcex_gamepad_buttons[port][7]  = inpDEVICE_GetKeyValue(device, 15) > 0.000001f;
        hcex_gamepad_buttons[port][5]  = inpDEVICE_GetKeyValue(device, 12) > 0.000001f;
        hcex_gamepad_buttons[port][4]  = inpDEVICE_GetKeyValue(device, 13) > 0.000001f;
        hcex_gamepad_buttons[port][8]  = inpDEVICE_GetKeyValue(device, 2)  > 0.000001f;
        hcex_gamepad_buttons[port][9]  = inpDEVICE_GetKeyValue(device, 3)  > 0.000001f;
        hcex_gamepad_buttons[port][10] = inpDEVICE_GetKeyValue(device, 4)  > 0.000001f;
        hcex_gamepad_buttons[port][11] = inpDEVICE_GetKeyValue(device, 5)  > 0.000001f;
        hcex_gamepad_buttons[port][12] = 0;
        hcex_gamepad_buttons[port][13] = inpDEVICE_GetKeyValue(device, 1)  > 0.000001f;
        hcex_gamepad_buttons[port][14] = inpDEVICE_GetKeyValue(device, 6)  > 0.000001f;
        hcex_gamepad_buttons[port][15] = inpDEVICE_GetKeyValue(device, 7)  > 0.000001f;
    }

    if ( !hcex_coop_local_player_index )
        hcex_raw_input_set(1, &hcex_coop_u1_inp_remote);
}
