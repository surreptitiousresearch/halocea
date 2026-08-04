#pragma once
#include "../ds/dsTYPE_ID.h"

// Boundary decls for aiBRAIN::implThrowWeaponsAndItems (src/ws/ai/aiBRAIN__implThrowWeaponsAndItems.cpp).
// The weapon/item list properties live in the wpn subsystem; only the runtime-type statics and the two
// "throw everything" entry points touched here are modeled. Their full layouts are the next frontier.

// wpnLIST — the weapon-list property (a propBASE-derived property found via propBASE::GetProperty).
struct wpnLIST {
    // 0x82F2C538 (?ThrowAllWeapons@wpnLIST@@UAAXXZ) — virtual: drop every held weapon.  boundary.
    void ThrowAllWeapons();
};

// wpnITEM_LIST — the carried-item-list property.
struct wpnITEM_LIST {
    // 0x831D6A20 (?implThrowAllItems@wpnITEM_LIST@@UAAXXZ) — virtual: drop every carried item.  boundary.
    void implThrowAllItems();
};

// Descriptor runtime-type statics tested by propContainer.typeId.IsDerivedFrom / GetProperty.  boundary.
struct wpnLIST_DESC {
    // ?TYPE_ID@wpnLIST_DESC@@2VdsTYPE_ID@@A — this class's runtime-type bitmask.
    static dsTYPE_ID TYPE_ID;
};
struct wpnITEM_LIST_DESC {
    // ?TYPE_ID@wpnITEM_LIST_DESC@@2VdsTYPE_ID@@A — this class's runtime-type bitmask.
    static dsTYPE_ID TYPE_ID;
};
