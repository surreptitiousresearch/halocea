/* hcex_dbg_add_float @0x823CEEC8 — append a "name = value" line to the HCEX debug write-buffer statistics
 * page, lazily registering the page on first use (hcexPage == -1 sentinel). Only writes when the page is
 * currently active. Float sibling of hcex_dbg_add_string; wb::RegisterPage / wb::IsActive / wb::Printf are
 * ws-engine debug write-buffer boundaries. */

extern int  hcexPage;
extern int  wb_RegisterPage(const char *name, int flags, int enabled);  /* wb::RegisterPage */
extern char wb_IsActive(int page);                                      /* wb::IsActive */
extern void wb_Printf(int page, const char *format, ...);               /* wb::Printf */

extern "C" void hcex_dbg_add_float(const char *name, float value)
{
    int page = hcexPage;
    if ( hcexPage == -1 )
    {
        page = wb_RegisterPage("0HCEX/show statistics", 0, 1);
        hcexPage = page;
    }
    if ( wb_IsActive(page) )
        wb_Printf(hcexPage, "%s = %0.3f\n", name, value);
}
