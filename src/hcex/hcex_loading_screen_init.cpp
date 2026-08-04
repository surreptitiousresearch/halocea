/* hcex_loading_screen_init @0x823F4950 — install HCEX_LOADING_SCREEN::Register as the ws-engine
 * UI-extension init hook (UI_EXT_INIT), so the loading-screen brand gets registered during
 * ui_new's normal init pass. */

#include "../headers/hcex/HCEX_LOADING_SCREEN.h"

extern void (*UI_EXT_INIT)();

void hcex_loading_screen_init()
{
    UI_EXT_INIT = HCEX_LOADING_SCREEN::Register;
}
