#include "../../headers/ws/gs/gsLVL_SYSTEM.h"

// ?gsSysLevel@@3PAVgsLVL_SYSTEM@@A — the single global gs-engine level-system message address
// (same extern convention used by entENTITY::Register).
extern gsLVL_SYSTEM *gsSysLevel;

// gsExtFindTplName @ 0x82794808
//
// Thin gs-namespace wrapper around the level system's FindTplName virtual: looks up an anim
// template by name, applying instAff's affix override, honoring noLoad to suppress an
// on-demand load if the template isn't resident.
animTPL *gsExtFindTplName(const char *name, dsAFFIX_STRING *instAff, int noLoad)
{
    return gsSysLevel->FindTplName(name, instAff, noLoad);
}
