#pragma once
// ws-engine ds: stateless deleter policy used as the DEL template argument to dsSHARED_PTR.
// DB-verified layout (types_members Deleter<gsOBJ_MODIFIER_BASE>): no data members — size 1
// (empty-class padding).

template<class T>
struct Deleter {
};
