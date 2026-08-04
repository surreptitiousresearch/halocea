#pragma once
// ssl subsystem: common base of every callable script element (native or compiled).
// DB-verified layout (types_members sslFUNCTION_BASE): pDecl@0, pClassOfDefinition@4,
// stateFunc@8 — size 12.

struct sslFUNCTION_DECL; // function/event signature (full definition in sslFUNCTION_DECL.h)
struct sslCLASS;         // ssl script class descriptor (full definition in sslCLASS.h)
struct sslOBJECT;        // ssl script object instance (full definition in sslOBJECT.h)
struct sslSTACK;         // ssl evaluation stack (full definition in sslSTACK.h)
struct sslERROR;         // ssl error/result value (full definition in sslERROR.h)

typedef struct sslFUNCTION_BASE {
    sslFUNCTION_DECL *pDecl;              // 0x00 declared signature
    sslCLASS         *pClassOfDefinition; // 0x04 class that declares this function (ref-counted)
    unsigned int      stateFunc;          // 0x08 load/parse state + flag bits (bit 0x2 = run during construct)

    // 0x82ADA548 — reorder `argc` named call arguments (values on the main stack, names on the
    // namedParams stack) into declared parameter order, pushing them back positionally with defaults
    // filled. Static (KA). Returns sslERROR by value (sret).
    static sslERROR TransferNamedParams(sslCLASS *pClass, int idx, int argc, sslSTACK &stack);

    // 0x82AD8418 — stateFunc bit 0x1 (meaning not otherwise established in this codebase; inferred
    // here as "owns pDecl") gates whether this element's pDecl is a privately-allocated
    // sslFUNCTION_DECL rather than one shared/borrowed from elsewhere; when set, tear down the
    // declaration's parameter vector and free it.
    ~sslFUNCTION_BASE();

protected:
    // 0x82AD9870 — normalise the `argc` call arguments on the operand stack to this function's
    // declared parameter list (defaults for missing, drop surplus, resolve refs, coerce types).
    // Returns sslERROR by value (sret). Protected (IAA).
    sslERROR ConvertParams(sslOBJECT *pSelf, int argc, sslSTACK &stack);
} sslFUNCTION_BASE;
