#pragma once

/* Boundary surface for txmTEX_DESC_DB::Init (src/ws/txm/txmTEX_DESC_DB__Init.cpp): the ".ps"
 * script loader and the per-descriptor parser, both external to this batch. */

#include "ENGINE_TEX_DESC.h"
#include "../ps/psSECTION.h"
#include "../ps/psSYSTEM.h" /* psSystem global root section */

/* ?txmTexDescLoad@@... -- parse an ENGINE_TEX_DESC out of a loaded ".td" psSECTION; returns
 * nonzero on success. boundary (ps-parser subsystem). */
extern int txmTexDescLoad(ENGINE_TEX_DESC *desc, const psSECTION *section);

/* psSYSTEM::LoadPsFromFile -- load and parse a ".ps"/".td" script file `fileName` (searched
 * under `subDir` with extension `ext`) relative to the `root` section, returning the parsed
 * result section by value. DEVIATION: the decompiler renders the sret + `this`/root argument
 * shuffle as a stack-reuse artifact (see psSYSTEM.h); modeled here as a free boundary wrapper
 * matching the observed call `LoadPsFromFile(<sret>, psSystem, fileName, subDir, ext, 0)`.
 * boundary (ps-parser subsystem, external to this batch). */
extern psSECTION psSYSTEM__LoadPsFromFile(psSECTION *root, const dsTSTRING<char> &fileName,
                                          const dsTSTRING<char> &subDir,
                                          const dsTSTRING<char> &ext, int flags);
