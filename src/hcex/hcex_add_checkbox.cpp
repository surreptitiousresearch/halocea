/* hcex_add_checkbox @0x823CE9D8 — dialog-setup stub: the incoming dialog handle is unused, and the
 * only effect is to force the "load as Halo" global on. This is the HCEX Windows-tool-side dialog
 * hook (mangled C++ free function, not the extern-C hcex_* bridge layer); the dialog controls
 * themselves are never populated here in this build. */

typedef struct HWND__ HWND__; /* opaque Win32 window handle target type; never dereferenced here */

extern int load_as_halo;

void hcex_add_checkbox(HWND__ *parent_hwnd)
{
    load_as_halo = 1;
}
