// HCEX_MODEL — a resolved Halo model instance: its ids/names plus per-node and per-region data.
// POD-ish boilerplate: default/copy ctor, dtor (no scalar deleting destructor is emitted for this
// class in the binary). See src/headers/hcex/HCEX_MODEL.h for the DB-verified layout.

#include "../../headers/hcex/HCEX_MODEL.h"

// 0x823D9834 — default-construct: haloName/hcexName adopt the shared empty-string singleton via
// UnsafeInitEmpty(), objNames/nodeIndices/regions adopt the ds_vector.h:24 call-site cookie
// (empty vectors). modelId is left uninitialized, matching the disassembly.
HCEX_MODEL::HCEX_MODEL()
    : objNames(apCL{"D:\\Projects\\code\\common\\INCL.SYS\\ds\\ds_vector.h", 24})
    , nodeIndices(apCL{"D:\\Projects\\code\\common\\INCL.SYS\\ds\\ds_vector.h", 24})
    , regions(apCL{"D:\\Projects\\code\\common\\INCL.SYS\\ds\\ds_vector.h", 24})
{
    haloName.pBuffer = nullptr;
    haloName.UnsafeInitEmpty();
    hcexName.pBuffer = nullptr;
    hcexName.UnsafeInitEmpty();
}

// 0x823E68F4 — copy-construct: copy modelId, share that.haloName's and that.hcexName's buffers
// (ref-bump each), then deep-copy objNames/nodeIndices/regions via Insert (adopting each source
// vector's call-site cookie).
HCEX_MODEL::HCEX_MODEL(const HCEX_MODEL &that)
    : modelId(that.modelId)
    , objNames(that.objNames.__cl)
    , nodeIndices(that.nodeIndices.__cl)
    , regions(that.regions.__cl)
{
    haloName.pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *haloBuffer = that.haloName.pBuffer;
    haloName.pBuffer = haloBuffer;
    ++haloBuffer->refCount;

    hcexName.pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *hcexBuffer = that.hcexName.pBuffer;
    hcexName.pBuffer = hcexBuffer;
    ++hcexBuffer->refCount;

    objNames.Insert(0, that.objNames.pData, that.objNames.nElem);
    nodeIndices.Insert(0, that.nodeIndices.pData, that.nodeIndices.nElem);
    regions.Insert(0, that.regions.pData, that.regions.nElem);
}

// 0x823D9980 — destructor body is empty: regions/nodeIndices/objNames/hcexName/haloName all
// destruct via the implicit per-member teardown, in reverse declaration order -- matching the
// disasm's regions-dtor-call, nodeIndices.pData-dlFree, objNames-dtor-call, hcexName-refcount-
// release, haloName-refcount-release sequence exactly (the two dsTSTRING releases and the POD
// nodeIndices vector teardown are inlined by the compiler; the two dsTSTRING<char>/HCEX_MODEL_REGION
// vector teardowns remain visible CALLs). No manual body is written here to avoid double-releasing
// any member.
HCEX_MODEL::~HCEX_MODEL()
{
}
