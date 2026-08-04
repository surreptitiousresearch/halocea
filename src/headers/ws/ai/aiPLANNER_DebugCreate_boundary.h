#pragma once
// Boundary decls used only by aiPLANNER::DebugCreateAIEntity (0x83195858) — the debug spawn
// helper that ray-casts from the camera and creates an AI entity at the hit point. The entity
// factory (entCreate) and its create-data are the next frontier; declared here as boundaries.

struct scnSCENE;
struct entENTITY;
struct animINST;
struct msgDATA;
struct camCAMERA;

// entCREATE_DATA — canonical DB-verified definition (animCREATE_DATA base + pDomSpawn@0xE8)
// lives in src/headers/entCREATE_DATA.h; the former 236-byte opaque echo here is retired.
#include "../../entCREATE_DATA.h"

// 0x82536ED8 ?entCreate@@YAPAVentENTITY@@PAVscnSCENE@@PBD1PAVentCREATE_DATA@@PAUmsgDATA@@@Z
extern entENTITY *entCreate(scnSCENE *scene, const char *className, const char *tplName,
                            entCREATE_DATA *data, msgDATA *msg);

// scene + camera + player accessors (boundaries, defined elsewhere).
extern camCAMERA *gsCameraGet(int idx);
extern entENTITY *gsGetPlayer(int index);
