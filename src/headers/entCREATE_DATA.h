#pragma once
/* entCREATE_DATA — 236-byte ws creation descriptor. DB-verified (types_members entCREATE_DATA):
   an animCREATE_DATA base (232B) plus pDomSpawn@0xE8 (232) — a dominant-spawn-info override
   consumed directly by entCreate(animINST*, entCREATE_INFO&). */

#include "ws/anim/animCREATE_DATA.h"

struct gsDOM_SPAWN; /* gs subsystem — dominant-spawn info (pointer only)   boundary */

struct entCREATE_DATA : animCREATE_DATA
{
    gsDOM_SPAWN *pDomSpawn;           /* 0xE8 (232) */

    entCREATE_DATA();                 /* 0x8252FFB8 */
    virtual ~entCREATE_DATA();        /* 0x823CE690 — delegates to ~animCREATE_DATA */
}; /* 236 bytes */
