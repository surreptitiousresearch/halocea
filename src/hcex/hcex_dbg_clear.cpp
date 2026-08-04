/* hcex_dbg_clear @ 0x823CEF60
   Clears the HCEX debug write-buffer page, unless no page is assigned (-1
   sentinel). wb::Clear is a ws-engine debug write-buffer boundary. */

extern int hcexPage;
extern void wb_Clear(int page);   /* wb::Clear(int) */

extern "C" void hcex_dbg_clear(void)
{
    if ( hcexPage != -1 )
        wb_Clear(hcexPage);
}
