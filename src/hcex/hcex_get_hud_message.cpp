/* hcex_get_hud_message @0x823D15B8 — resolve a localized HUD message string. Builds a string-table key from
 * the message/element indices — either the global "hud_globals_%d_%d" form or the per-level
 * "<level>_hud_messages_%d_%d" form (level name pulled from haloEngineCtrl->curLevel) — and looks it up via
 * hcex_get_string. _snprintf_0 is the CRT-style boundary sprintf; hcex_get_string returns the wide string. */

#include <wchar.h>
#include "../headers/hcex/haloENGINE_CONTROL.h"

extern "C" int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern wchar_t *hcex_get_string(const char *str_name);
extern haloENGINE_CONTROL *haloEngineCtrl;

extern "C" wchar_t *hcex_get_hud_message(int is_glob, int message_idx, int elem_idx)
{
    char string_key[136];

    if ( is_glob )
        _snprintf_0(string_key, 0x80, "hud_globals_%d_%d", message_idx, elem_idx);
    else
        _snprintf_0(string_key, 0x80, "%s_hud_messages_%d_%d",
                   haloEngineCtrl->curLevel.pBuffer->str, message_idx, elem_idx);

    return hcex_get_string(string_key);
}
