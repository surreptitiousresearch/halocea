#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"
#include "../../headers/ws/ap/apPROCESSOR_LOCK.h"

// 0x823C4580 — copy-construct: adopt `cb`/`id` verbatim, then share `other.sslDecl`'s buffer
// (bump its refcount) under the dsTSTRING<char> processor-affinity guard, matching every other
// dsTSTRING-copying ctor in this corpus.
SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry::SSLExportEntry(const SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry &other)
{
    cb = other.cb;
    id = other.id;

    sslDecl.pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *otherBuffer = other.sslDecl.pBuffer;
    dsTSTRING<char>::processorLock.CheckCurrentProcessor(
        "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp", 883);
    sslDecl.pBuffer = otherBuffer;
    ++sslDecl.pBuffer->refCount;
}
