/* ?GetLinesNumber@UI_STAT_SYS@ui_new@@AAAHPB_W@Z @0x82CED430 */
#include "../../headers/ws/ui_new/UI_STAT_SYS.h"
#include <wchar.h>

// 0x82CED448 — count '\n'-delimited lines in `str` (always at least 1).
int ui_new::UI_STAT_SYS::GetLinesNumber(const wchar_t *str)
{
    int lines = 1;
    for (const wchar_t *p = wcschr(str, L'\n'); p; p = wcschr(p + 1, L'\n'))
        ++lines;
    return lines;
}
