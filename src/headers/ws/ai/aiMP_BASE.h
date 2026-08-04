#pragma once
// ws-engine ai08: base of a brain's mount-point / movement-prediction module (aiMIND::mp pointee).
// sizeof=0x4 — only the vtbl pointer. The process-wide default (aiMIND::GetDefSys_Mp) is a bare
// aiMP_BASE whose ctor sets only __vftable.  boundary (full vtbl is the next frontier).

struct aiMP_BASE_vtbl;

struct aiMP_BASE {
    aiMP_BASE_vtbl *__vftable; // 0x00
};
