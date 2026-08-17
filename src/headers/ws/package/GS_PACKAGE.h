#pragma once
// ws-engine package: the game content-package manager (DLC / add-on texture packs). It is a
// message-system actor (iaIACTOR base) tracking a new and an old package set; each set entry
// (GS_PACKAGE_INFO) names the level(s) it covers and carries the texture names + loaded texture
// handles for that package. ProcessLoadTextures streams the textures for the current package.
//
// DB-verified layouts (types_members package::GS_PACKAGE size 260, package::GS_PACKAGE_INFO).

#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
#include "../ap/apSTATE_T.h"
#include "../txm/txmTEXTURE_PTR.h"
#include "../ia/iaIACTOR.h"
#include "PKG_MNG_STATES.h"

namespace package {

// One package's texture manifest. DB: levelNames@0, textures@20, textureNames@40, pkgIdx@60,
// pkgName@64, pkgMask@68 (size 72).
struct GS_PACKAGE_INFO {
    dsVECTOR<dsSTRID, 8>            levelNames;   // 0x00 levels this package applies to
    dsVECTOR<txmTEXTURE_PTR, 8>    textures;     // 0x14 loaded texture handles
    dsVECTOR<dsTSTRING<char>, 8>   textureNames; // 0x28 texture asset names (no path/extension)
    int                            pkgIdx;       // 0x3C package index
    dsTSTRING<char>                pkgName;      // 0x40 package file name
    unsigned char                  pkgMask;      // 0x44 DLC bit mask
};

struct GS_PACKAGE : iaIACTOR {
    dsVECTOR<GS_PACKAGE_INFO, 8>     newList;             // 0xC8 (200)
    dsVECTOR<GS_PACKAGE_INFO, 8>     oldList;             // 0xDC (220)
    apSTATE_T<unsigned long>         state;               // 0xF0 (240)
    int                              curIdx;              // 0xF4 (244) current package index
    dsTSTRING<char>                  curRoot;             // 0xF8 (248) current pak root path string
    int                              isReady;             // 0xFC (252)
    unsigned char                    dlcMask;             // 0x100 (256)

    // 0x82778590 — for the current package, build "\textures\<name>.pct" paths, read each into a
    // memory file and register/load it through txmManager, then close the pak.
    void ProcessLoadTextures();
};

// The package/pak file manager the loader closes through. boundary — the concrete instance is
// package::PKG_MNG_XENON (PKG_MNG@0, size 80), Close(dsTSTRING<char>, int) is virtual
// (?Close@PKG_MNG_XENON@package@@UAAXV?$dsTSTRING@D@@H@Z).
// DB-verified layout (types_members package::PKG_MNG): __vftable@0, state@4 — size 8.

struct PKG_MNG;

// DB-verified slots (types_members package::PKG_MNG_vtbl, 6 slots, size 24). The dtr slot models
// the deleting-destructor flag per the corpus DTR CONVENTION; OpenByIdx returns a dsTSTRING by
// value, spelled result-pointer-first per this binary's verified sret ABI (r3=result, r4=this).
struct PKG_MNG_vtbl {
    void (*dtr_PKG_MNG)(PKG_MNG *self, int freeFlags);                                    // 0x00
    int  (*FillList)(PKG_MNG *self, int a2);                                              // 0x04
    int  (*GetCount)(PKG_MNG *self);                                                      // 0x08
    dsTSTRING<char> *(*OpenByIdx)(dsTSTRING<char> *result, PKG_MNG *self, int a3, int a4);// 0x0C
    void (*Close)(PKG_MNG *self, dsTSTRING<char> name, int flags);                        // 0x10
    PKG_MNG_STATES (*ProcessThink)(PKG_MNG *self);                                        // 0x14
};

struct PKG_MNG {
    PKG_MNG_vtbl    *__vftable;   // 0x00
    PKG_MNG_STATES   state;       // 0x04 fill-list/open/close state machine
    void Close(dsTSTRING<char> name, int flags); // virtual dispatch
};
extern PKG_MNG *pkg_mng; // package::pkg_mng

} // namespace package
