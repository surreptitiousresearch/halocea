/* hcex_dbg_add_string @0x823CEE48 — append a line to the HCEX debug write-buffer statistics page, lazily
 * registering the page on first use (hcexPage == -1 sentinel). Only writes when the page is currently active.
 * wb::RegisterPage / wb::IsActive / wb::Printf are ws-engine debug write-buffer boundaries; pairs with
 * hcex_dbg_clear (wb::Clear). */

extern int hcexPage;
extern int  wb_RegisterPage(const char *name, int flags, int enabled);  /* wb::RegisterPage */
extern char wb_IsActive(int page);                                      /* wb::IsActive */
extern void wb_Printf(int page, const char *format, ...);               /* wb::Printf */

extern "C" void hcex_dbg_add_string(const char *string)
{
    int page = hcexPage;
    if ( hcexPage == -1 )
    {
        page = wb_RegisterPage("0HCEX/show statistics", 0, 1);
        hcexPage = page;
    }
    if ( wb_IsActive(page) )
        wb_Printf(hcexPage, "%s", string);
}
