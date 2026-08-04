#pragma once
#include "../ds/dsSTRID.h"
// ssl subsystem: one member/field descriptor of a script class (element of
// dsVECTOR<sslCLASS_ELEMENT,8> sslCLASS::elements). DB-verified layout
// (types_members sslCLASS_ELEMENT): name@0 (dsSTRID), type@4 (u8), isInherited@5 (u8),
// anonymous union@8 (4 bytes) — size 12.

struct sslCB_FUNCTION;        // boundary — native callback binding (TYPE_CB_FUNC)
struct sslSCRIPTED_FUNCTION;  // boundary — compiled script function (TYPE_SSL_FUNC)
struct sslEVENT_HANDLER;      // boundary — event handler (TYPE_EVENT)
struct sslBASE_VAR;           // boundary — variable/constant slot (TYPE_CONST)

typedef struct sslCLASS_ELEMENT {
    // Element kind discriminator (the `type` byte). DB enum sslCLASS_ELEMENT::TYPE
    // (types_enum_values; DB spells value 2 TYPE_SSL_EVENT — local alias TYPE_EVENT kept).
    enum TYPE {
        TYPE_CB_FUNC       = 0, // native callback function (pCbFunc)
        TYPE_SSL_FUNC      = 1, // scripted function (pSslFunc)
        TYPE_EVENT         = 2, // event handler (pSslEvHand); DB name TYPE_SSL_EVENT
        TYPE_CB_PROPERTY   = 3, // native property (pVar-backed)
        TYPE_STORED_MEMBER = 4, // stored member variable (pVar-backed)
        TYPE_CONST         = 5, // constant / base variable (pVar)
    };

    dsSTRID       name;        // 0x00 element name
    unsigned char type;        // 0x04 TYPE discriminator
    unsigned char isInherited; // 0x05 declared on a base class
    unsigned char _pad06[2];   // 0x06 db-verified padding

    // 0x08 element payload (DB anonymous union _2F379955AB8216B4674A21FBF78CED8F, 4 bytes) — arms
    // injected into struct scope so bodies access pVoid/pCbFunc/pSslFunc/pVar directly.
    union {
        void                  *pVoid;      // 0x00
        sslCB_FUNCTION        *pCbFunc;    // 0x00 TYPE_CB_FUNC
        sslSCRIPTED_FUNCTION  *pSslFunc;   // 0x00 TYPE_SSL_FUNC
        sslEVENT_HANDLER      *pSslEvHand; // 0x00 TYPE_EVENT
        sslBASE_VAR           *pVar;       // 0x00 TYPE_CONST
    };
} sslCLASS_ELEMENT;
