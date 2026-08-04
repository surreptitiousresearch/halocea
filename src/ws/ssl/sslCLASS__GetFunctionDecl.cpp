#include "sslCLASS.h"
#include "sslCLASS_ELEMENT.h"
#include "sslFUNCTION_DECL.h"

// 0x82A3AD78 — sslCLASS::GetFunctionDecl. For a function (native/scripted) or event element at
// `idx`, return the sslFUNCTION_DECL pointer stored at the front of the element's payload; nullptr
// otherwise. The inclusive `idx <= elements.nElem` upper bound is reproduced faithfully from the
// binary.
sslFUNCTION_DECL *sslCLASS::GetFunctionDecl(int idx)
{
    if (idx >= 0 &&
        ((idx <= this->elements.nElem &&
          (this->elements[idx].type == sslCLASS_ELEMENT::TYPE_CB_FUNC ||
           this->elements[idx].type == sslCLASS_ELEMENT::TYPE_SSL_FUNC)) ||
         (idx <= this->elements.nElem &&
          this->elements[idx].type == sslCLASS_ELEMENT::TYPE_EVENT)))
    {
        // The function/event descriptor stores its sslFUNCTION_DECL* as its first field.
        return *reinterpret_cast<sslFUNCTION_DECL **>(this->elements[idx].pVoid);
    }
    return nullptr;
}
