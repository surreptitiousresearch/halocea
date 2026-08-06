/* hcex_dbg_print_outgoing_values @0x823D3560 — dump the four "weapon outgoing" values to the HCEX debug
 * write-buffer statistics page, lazily registering the page on first use (hcexPage == -1 sentinel) and only
 * writing when the page is active. Sibling of hcex_dbg_add_float; wb::RegisterPage / wb::IsActive /
 * wb::Printf are ws-engine debug write-buffer boundaries; dsSPrintf / dsTSTRING_flat are ds boundaries.
 *
 * DEVIATION: the decompiler fabricated a `double a2` parameter (a float-slot-skip ABI artifact) and printed
 * it; the disasm (`lfs f0,0(r28)` -> stored as the %.3f arg) shows each line prints the loaded values[i], so
 * the real prototype takes only the array pointer. */

#include "../headers/hcex/hcex_ds_boundary.h"   /* dsTSTRING_flat, dsSPrintf, dlFree */

extern "C" int  hcexPage;
extern int  wb_RegisterPage(const char *name, int flags, int enabled);  /* wb::RegisterPage */
extern char wb_IsActive(int page);                                      /* wb::IsActive */
extern void wb_Printf(int page, const char *format, ...);               /* wb::Printf */

extern "C" void hcex_dbg_print_outgoing_values(const float *values)
{
    for ( int i = 0; i < 4; ++i )
    {
        double value = values[i];
        dsTSTRING_flat line;
        dsSPrintf(&line, "weapon outgoing [%d] = %.3f\n", i, value);
        const char *str = line.pBuffer->str;

        int page = hcexPage;
        if ( hcexPage == -1 )
        {
            page = wb_RegisterPage("0HCEX/show statistics", 0, 1);
            hcexPage = page;
        }
        if ( wb_IsActive(page) )
            wb_Printf(hcexPage, "%s", str);

        if ( --line.pBuffer->refCount == 0 )
            dlFree(line.pBuffer);
    }
}
