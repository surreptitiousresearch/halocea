#pragma once
// ws-engine sml: a resolved state id — an index into the shared sml::fctState name registry
// (see STATE_DESC.h). DB-verified layout (types_members smlSTATE_ID): val@0 (int) — size 4.

typedef struct smlSTATE_ID {
    int val; // 0x00 index into sml::fctState, or -1 when unresolved
} smlSTATE_ID;
