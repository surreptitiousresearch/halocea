#pragma once

// ws-engine ai08 — alert kind bit-flags raised on a brain/mind (see aiMIND::PostDelayedOnAlert,
// aiBRAIN::OnAlert). DB-verified values (types_enum_values AI_ALERT).
enum AI_ALERT {
    AI_ALERT_SEE         = 0x00000001,
    AI_ALERT_HEAR        = 0x00000002,
    AI_ALERT_DAMAGE      = 0x00000004,
    AI_ALERT_PHYS_OBJ    = 0x00000008,
    AI_ALERT_INTEROP     = 0x00000100,
    AI_ALERT_FROM_SCRIPT = 0x00010000,
    AI_ALERT_GRAVLINK    = 0x00100000,
};
