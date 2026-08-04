#pragma once
// ws-engine wpn: a weapon's ammo clip — tracks current/max clip ammo. Only the query surface
// reached by aiRELOADER::IsLoaded is declared here; the full layout/bodies are the next frontier.
// boundary.

struct wpnAMMO_CLIP {
    // 0x83153670 (?IsNoClip@wpnAMMO_CLIP@@QBA_NXZ) — const: true for a "no clip" weapon (infinite /
    // clip-less ammo), for which the percent-loaded test is skipped.  boundary.
    bool IsNoClip() const;
    // 0x83153378 (?GetAmmoClipCur@wpnAMMO_CLIP@@QBAMXZ) — const: current ammo in the clip.  boundary.
    float GetAmmoClipCur() const;
    // 0x83153380 (?GetAmmoClipMax@wpnAMMO_CLIP@@QBAMXZ) — const: clip capacity.  boundary.
    float GetAmmoClipMax() const;
};
