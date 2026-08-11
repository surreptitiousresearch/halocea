#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// @0x823C3810 — releases the sslDecl string buffer reference. The decompile shows an explicit
// `dsTSTRING<char>::~dsTSTRING<char>(&this->sslDecl)` call, which is simply how the compiler
// implements member destruction — idiomatically this is just the implicit member-dtor run after
// an empty body. `cb`/`id` need no cleanup (raw pointers, not owned).
SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry::~SSLExportEntry()
{
}
