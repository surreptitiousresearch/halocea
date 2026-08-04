#pragma once
// ws-engine (Saber) m3d math: 3-component vector.
// DB-verified layout (types_members m3dV): x@0, y@4, z@8 — all float, size 12.

typedef struct m3dV {
    float x;
    float y;
    float z;

    // 0x823CD3C8 (??Ym3dV@@QAAAAU0@ABU0@@Z) — component-wise add-assign; returns *this.
    // (A COMDAT-folded copy also lives at 0x8265B2C8, used by animINST::UpdateSkinCompOrigin.)
    // REVERSED: src/ws/m3d/m3dV__operatorAddAssign.cpp
    m3dV &operator+=(const m3dV &rhs);

    // 0x82561E78 (??_0m3dV@@QAAAAU0@M@Z) — component-wise scalar divide-assign.  boundary
    // (used by m3dVRand)
    m3dV &operator/=(float scalar);

    // ??X m3dV::operator*=(float) — component-wise scalar multiply-assign.  boundary
    m3dV &operator*=(float scalar);
} m3dV;
