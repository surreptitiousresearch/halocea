#pragma once
/* entCREATE_DATA — 236-byte ws creation descriptor. DB-verified (types_members entCREATE_DATA):
   row 0 is an ANONYMOUS BASE-CLASS member of type animCREATE_DATA at offset 0 (is_baseclass=1,
   232B) — hence the inheritance below, not a named `base` member — plus pDomSpawn@0xE8 (232), a
   dominant-spawn-info override consumed directly by entCreate(animINST*, entCREATE_INFO&).
   Canonical home, and as of 2026-08-07 the SOLE definition: hcex/hcex_create_effect_impl_boundary.h
   carried a flat `animCREATE_DATA base;` copy and now includes this header (odr_dup drain). */

#include "ws/anim/animCREATE_DATA.h"

struct gsDOM_SPAWN; /* gs subsystem — dominant-spawn info (pointer only)   boundary */

struct entCREATE_DATA : animCREATE_DATA
{
    gsDOM_SPAWN *pDomSpawn;           /* 0xE8 (232) */

    entCREATE_DATA();                 /* 0x8252FFB8 */
    ~entCREATE_DATA();        /* 0x823CE690 — delegates to ~animCREATE_DATA */
}; /* 236 bytes */
