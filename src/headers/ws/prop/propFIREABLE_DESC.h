#pragma once
// ws-engine prop: descriptor for a fireable/health component. BOUNDARY slice — only the surface the
// ai08 aiPROP_FIREABLE damage path reaches is declared. Full DB layout (headers_ref
// propFIREABLE_DESC, size 0xB4) is a later frontier; no data-member offsets are asserted here.

struct dmgTYPE;
struct gsGEOM_ANCHOR;

struct propFIREABLE_DESC {
    // ?IsAcceptDamage@propFIREABLE_DESC@@... — descriptor-level accept/reject test for an incoming
    // hit of type `pDmgType` landing at `hitInfo`; `isDirectHit` distinguishes direct vs splash.
    // boundary (body external to this batch).
    bool IsAcceptDamage(const dmgTYPE *pDmgType, gsGEOM_ANCHOR *hitInfo, int isDirectHit);
};
