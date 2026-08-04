#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// 0x823C4530 — compiler-synthesized `scalar deleting destructor`: runs the real destructor, then
// (per bit 0 of the synthesized delete-flag argument) `operator delete`s this.
SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry *
SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry::ScalarDeletingDestructor(unsigned int deleteFlag)
{
    this->~SSLExportEntry();
    if ((deleteFlag & 1) != 0)
        operator delete(this);
    return this;
}
