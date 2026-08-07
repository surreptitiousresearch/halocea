#pragma once
// ws-engine wpn: a weapon's ammo clip — tracks current/max clip ammo. Only the query surface
// reached by aiRELOADER::IsLoaded is declared here; the full layout/bodies are the next frontier.
// boundary.
// DB layout for that frontier (types_members wpnAMMO_CLIP, size 24): dsREF_COUNT base@0,
// spDesc@4 dsSMART_PTR<wpnAMMO_CLIP_DESC,…>, ammoClipCur@8 float, ammoClipMax@12 float,
// isUnlimitedAmmo@16 bool, spStorage@20 dsSMART_PTR<wpnRESERVE_AMMO_STORAGE,…>. Deliberately
// modelled opaque: every use in the tree is a pointer (dsSMART_PTR<wpnAMMO_CLIP,…>::pointee in
// aiRELOADER::IsLoaded, a fwd decl in wpnSHOOTER.h), so no TU depends on the size or layout.

struct wpnAMMO_CLIP {
    // 0x83153670 (?IsNoClip@wpnAMMO_CLIP@@QBA_NXZ) — const: true for a "no clip" weapon (infinite /
    // clip-less ammo), for which the percent-loaded test is skipped.  boundary.
    bool IsNoClip() const;
    // 0x83153378 (?GetAmmoClipCur@wpnAMMO_CLIP@@QBAMXZ) — const: current ammo in the clip.  boundary.
    float GetAmmoClipCur() const;
    // 0x83153380 (?GetAmmoClipMax@wpnAMMO_CLIP@@QBAMXZ) — const: clip capacity.  boundary.
    float GetAmmoClipMax() const;
};
