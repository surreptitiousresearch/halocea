#include "sslERROR.h"

// sslERROR::sslERROR() @ 0x823C5B88
// Default (no-error) construct: point desc at the process-wide shared empty-string singleton
// (lazily allocated on first use, per dsTSTRING<char>::UnsafeInitEmpty -- the DB shows this
// inlined as a guarded local-static init; modeled here as the equivalent UnsafeInitEmpty call,
// same convention as apDATA_TRACKER::RECORD::RECORD / con::CONSOLE_CALLBACK::CONSOLE_CALLBACK).
sslERROR::sslERROR()
{
    this->desc.pBuffer = nullptr;
    this->desc.UnsafeInitEmpty();
    this->tags = 0;
    this->id = SSL_ERR_NO_ERROR;
}
