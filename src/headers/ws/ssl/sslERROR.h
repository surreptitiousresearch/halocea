#pragma once
#include "../ds/dsTSTRING.h"
// ssl subsystem: a script error/result value. DB-verified layout (types_members sslERROR):
//   id@0 (sslERROR_ID), desc@4 (dsTSTRING<char>), tags@8 (int) — size 12.

// DB-verified full enumerator set (types_enum_values sslERROR_ID). 0 = no error.
enum sslERROR_ID {
    SSL_ERR_NO_ERROR                = 0, // (0 = no error)
    SSL_ERR_GENERIC                 = 1, // formatted diagnostic error (id passed to the (id,desc,tags) ctor)
    SSL_ERR_FILEIO                  = 2,
    SSL_ERR_SYNTAX                  = 3,
    SSL_ERR_UNKNOWN_NAME            = 4,
    SSL_ERR_DUPLICATE_NAME          = 5,
    SSL_ERR_TYPE_CHECK              = 6,
    SSL_ERR_RUNTIME                 = 7,
    SSL_ERR_LOADING                 = 8,
    _SSL_ERR_EVENT_HANDLER_REJECTED = 9,
    _SSL_ERR_EVENT_HANDLER_ABORT    = 10,
};

typedef struct sslERROR {
    sslERROR_ID     id;   // 0x00 error code
    dsTSTRING<char> desc; // 0x04 human-readable description
    int             tags; // 0x08 error classification flags

    // 0x823C5B88 — default (no-error) construct: adopt the shared empty-string singleton for desc.
    sslERROR();
    // 0x825CBB00 — formatted error construct: adopt the shared empty singleton first, then share
    // `desc`'s buffer (unless `desc` aliases this->desc); logs the message via sslLogCb when `id`
    // is a real error (non-zero).
    sslERROR(sslERROR_ID id, const dsTSTRING<char> &desc, int tags);
    // 0x823C4F38 — ref-counted assign (field-wise; desc via dsTSTRING::operator=).
    sslERROR &operator=(const sslERROR &other);
} sslERROR;
