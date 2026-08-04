/* HCEX_LOADING_SCREEN::HCEX_LOADING_SCREEN @0x823F3A80 — chain to ui_new::UI_FRAME's ctor, set
 * this class's own vtable, zero loadingScreenProgress, and create the shared D3D resources. */

#include "../headers/hcex/HCEX_LOADING_SCREEN.h"

extern void HCEXLoadingScreenCreateResources();

HCEX_LOADING_SCREEN::HCEX_LOADING_SCREEN() : ui_new::UI_FRAME()
{
    this->__vftable = (ui_new::UI_FRAME_vtbl *)&HCEX_LOADING_SCREEN::vftable; // base __vftable type
    loadingScreenProgress = 0.0f;
    HCEXLoadingScreenCreateResources();
}
