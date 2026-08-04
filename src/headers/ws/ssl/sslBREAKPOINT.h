#pragma once
// ssl subsystem: one script breakpoint record (element of sslSOURCE::Breakpoints and
// sslSYSTEM::bpData). DB-verified layout (types_members sslBREAKPOINT):
//   line@0, class_uid@4, object_uid@8 — size 12.

typedef struct sslBREAKPOINT {
    int line;       // 0x00 source line the breakpoint is on
    int class_uid;  // 0x04 class uid the breakpoint is scoped to (-1 = any)
    int object_uid; // 0x08 object uid the breakpoint is scoped to (-1 = any)
} sslBREAKPOINT;
