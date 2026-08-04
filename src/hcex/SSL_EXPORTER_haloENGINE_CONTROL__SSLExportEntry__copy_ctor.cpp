#include "../headers/hcex/ssl_exporter/SSLExportEntry.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsTSTRING_BUF_HEADER.h"
#include "../headers/ws/ap/apPROCESSOR_LOCK.h"

// SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry::SSLExportEntry(const SSLExportEntry&) @ 0x823C4560
// Copy-construct: adopt cb/id verbatim and share `other`'s sslDecl buffer (ref-counted), under the
// dsTSTRING processor-affinity lock.
SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry::SSLExportEntry(const SSLExportEntry &other)
{
    this->cb = other.cb;
    this->id = other.id;
    this->sslDecl.pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *buf = other.sslDecl.pBuffer;
    dsTSTRING<char>::processorLock.CheckCurrentProcessor(
        "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp", 883);
    this->sslDecl.pBuffer = buf;
    ++this->sslDecl.pBuffer->refCount;
}
