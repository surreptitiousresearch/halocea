/* HCEX_LOADING_SCREEN::Register @0x823F4880 — register the "hcex_loading_screen" brand
 * (parented under "ui_iface") with ui_new::uiFamily, and set its constructor thunk to
 * dsMakeFunc<void,HCEX_LOADING_SCREEN>. Mirrors the brand-registration idiom used throughout
 * haloInit for the Blam-side brands (entLIGHT_DESC, HALO_MNG_DESC, ...). */

#include "../headers/hcex/HCEX_LOADING_SCREEN.h"
#include "../headers/hcex/HCEX_LOADING_SCREEN_DESC.h" // RegisterBrand<HCEX_LOADING_SCREEN_DESC> template arg
#include "../headers/ws/dsc/dscDESC_FAMILY.h"

namespace ui_new { struct UI_FAMILY : dscDESC_FAMILY {}; extern UI_FAMILY *uiFamily; }

template<class R, class T> R *dsMakeFunc(const char *file, int line); // boundary — factory-thunk template

void HCEX_LOADING_SCREEN::Register()
{
    dsTSTRING<char> nameParent;
    nameParent.pBuffer = nullptr;
    nameParent.UnsafeInit("ui_iface", -1, 0);

    dsTSTRING<char> name;
    name.pBuffer = nullptr;
    name.UnsafeInit("hcex_loading_screen", -1, 0);

    dscBRAND *brand = ui_new::uiFamily->RegisterBrand<HCEX_LOADING_SCREEN_DESC>(name, nameParent);
    if (brand)
        brand[1].__vftable = (dscBRAND_vtbl *)dsMakeFunc<void, HCEX_LOADING_SCREEN>;

    /* name/nameParent release their shared buffers via ~dsTSTRING() at scope exit, matching the
     * decompiler's inlined refcount-decrement-then-dlFree tail for each. */
}
