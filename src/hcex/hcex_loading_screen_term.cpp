/* hcex_loading_screen_term @0x823F3F88 — thin thunk forwarding to HCEXLoadingScreenDestroyResources. */

extern void HCEXLoadingScreenDestroyResources();

void hcex_loading_screen_term()
{
    HCEXLoadingScreenDestroyResources();
}
