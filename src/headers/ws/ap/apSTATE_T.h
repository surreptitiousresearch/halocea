#pragma once
// ws-engine ap: state-holder wrapper. DB-verified layout (types_members apSTATE_T<unsigned long>):
//   state@0 (unsigned int) — size 4. Modeled as a template; the storage is the template argument.

template<class T>
struct apSTATE_T {
    T state; // 0x00 current state value
};
