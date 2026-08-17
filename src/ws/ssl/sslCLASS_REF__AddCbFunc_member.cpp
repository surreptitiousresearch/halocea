#include "sslCLASS_REF.h"
#include "sslCLASS.h"
#include "sslERROR.h"
#include "../ds/dsTSTRING.h"

// Re-source boundaries one level past this method (not descended this wave): the SSL lexer and
// parser, and the sslERROR hint-logging helpers. Declared here with the shapes observed at the
// call sites so this TU is self-contained.
struct sslLEX;
struct sslPARSER;
dsTSTRING<char> dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);
// sslPARSER::AddCbFuncToClass (member-callback overload, 0x82A447F0) — boundary.
// DEVIATION: previously declared inside an anonymous namespace (internal linkage, never
// defined, odr-used below — unresolvable at link); hoisted to file scope like the
// sibling boundary decls.
extern sslERROR sslPARSER_AddCbFuncToClass(sslPARSER *self, sslCLASS_REF cls, sslLEX &lex,
                                           sslCB_MEMBER_FN cbFunc, int *idx, int flags);
// sslLEX(const char *src, int maxLen) ctor / dtor, sslPARSER(sslSYSTEM *) ctor — boundary.
extern void sslLEX_ctor(sslLEX *self, const char *src, int maxLen);
extern void sslLEX_dtor(sslLEX *self);
extern void sslPARSER_ctor(sslPARSER *self, sslSYSTEM *sys);
// sslERROR::GetStr(dsTSTRING<char>&) const / sslERROR::LogErr(const dsTSTRING<char>&, int) — boundary.
extern const dsTSTRING<char> &sslERROR_GetStr(const sslERROR *self, dsTSTRING<char> *scratch);
extern void sslERROR_LogErr(const dsTSTRING<char> &line, int tags);

// sslCLASS_REF::AddCbFunc @ 0x82524DD0 — member-callback overload. Registers the pointer-to-member
// native callback `cbFunc`, under the function declaration text `decl`, on the referenced script
// class. Returns an sslERROR by value (sret; the decompiler renders the hidden result pointer as
// `this` and the real object as the first explicit parameter).
sslERROR sslCLASS_REF::AddCbFunc(const char *decl, sslCB_MEMBER_FN cbFunc,
                                 const char *errHint, int *idx)
{
    if (!this->pClass)
    {
        // Empty class: raise a generic error (the ctor logs it), then append the hint if any.
        sslERROR err(SSL_ERR_GENERIC, dsTSTRING<char>("trying to add cb-func to empty class"), 0);
        if (err.id && errHint && *errHint)
        {
            dsTSTRING<char> hint;
            sslERROR_LogErr(dsSPrintf(&hint, "Hint: %s", errHint), err.tags);
        }
        return err;
    }

    // Parse `decl` and hand off to the parser bound to this class's owning script system.
    char lexStorage[64];
    sslLEX *lex = reinterpret_cast<sslLEX *>(lexStorage);
    sslLEX_ctor(lex, decl, 0x7FFFFFFF);

    sslSYSTEM *pSslSystem = this->pClass ? this->pClass->pSslSystem : nullptr;
    char parserStorage[32];
    sslPARSER *parser = reinterpret_cast<sslPARSER *>(parserStorage);
    sslPARSER_ctor(parser, pSslSystem);

    sslCLASS_REF classRef(*this);
    sslERROR err = sslPARSER_AddCbFuncToClass(parser, classRef, *lex, cbFunc, idx, 0);

    if (err.id && errHint)
    {
        dsTSTRING<char> scratch, line;
        sslERROR_LogErr(dsSPrintf(&line, "Hint: %s %s", errHint,
                                  sslERROR_GetStr(&err, &scratch).pBuffer->str),
                        err.tags);
    }

    sslLEX_dtor(lex);
    return err;
}
