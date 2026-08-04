/* HCEX_LOADING_SCREEN::~HCEX_LOADING_SCREEN @0x823F3B20 — reset __vftable to this class's own
 * (defensive re-stamp before the base dtor runs, matching the disasm), then chain to
 * ~ui_new::UI_FRAME implicitly via the base-destructor call at scope exit. */

#include "../headers/hcex/HCEX_LOADING_SCREEN.h"

HCEX_LOADING_SCREEN::~HCEX_LOADING_SCREEN()
{
    this->__vftable = (ui_new::UI_FRAME_vtbl *)&HCEX_LOADING_SCREEN::vftable; // base __vftable type
}
