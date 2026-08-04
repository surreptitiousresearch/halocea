#include "../headers/ws/ds/dsDEQUE.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// dsDEQUE<haloPLAYER_ACTIONS_ENV>::dsDEQUE(apCL) @ 0x823BF790
// Empty-deque ctor: default-construct the backing vector (null storage, zero count, adopting the
// allocation call-site cookie `cl`) then zero the ring indices and length.
// DEVIATION: the PPC decompiler renders the by-value apCL member copy as two field writes assigning
// `this` to data.__cl.file and `cl` to data.__cl.line; that is an artifact of it not modeling the
// apCL copy performed by the inlined dsVECTOR(const apCL&) ctor. Real semantics: `data.__cl = cl`.
template<>
dsDEQUE<haloPLAYER_ACTIONS_ENV>::dsDEQUE(apCL cl)
{
    this->data.pData = nullptr;
    this->data.nElem = 0;
    this->data.allocated = 0;
    this->data.__cl = cl;
    this->head = 0;
    this->tail = 0;
    this->length = 0;
}
