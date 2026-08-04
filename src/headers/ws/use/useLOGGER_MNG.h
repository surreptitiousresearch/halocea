#pragma once
#include "../ap/apSTATE_T.h"
// useLOGGER_MNG — ws-engine "usables" logger/manager (use_logger.h/.cpp). BOUNDARY receiver: only
// the members past gameIsPaused (@232) plus gameIsPaused itself are modeled at their DB-verified
// offsets (types_members useLOGGER_MNG, re-checked 2026-08-04); the leading region is an opaque
// blob sized to the DB offsets. Derives from con::CONSOLE_CALLBACK (16 bytes) at offset 0.
struct useLOGGER_MNG {
    char _pad0[232];      // 0x00 con::CONSOLE_CALLBACK base + turnedOn + usablesEnt + ...
    bool gameIsPaused;    // 0xE8 (232)
    char _padE9[3];       // 0xE9
    unsigned int             lastSelectedItem;     // 0xEC (236)
    apSTATE_T<unsigned long> lastSelectedItemType; // 0xF0 (240) — total size 244

    bool IsGamePaused(); // 0x827848F0
};
