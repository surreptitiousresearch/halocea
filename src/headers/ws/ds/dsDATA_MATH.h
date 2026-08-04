#pragma once
// ws-engine ds_data: enumerations for dsDATA::Math (unary arithmetic on a type-erased value).
// DB-verified values (types_enum_values dsDATA_MATH_RES / dsDATA_UNARY_OP).

// Result of a unary math dispatch.
enum dsDATA_MATH_RES {
    DSD_MATH_UNDEF  = -1, // no math operator registered for the stored type
    DSD_MATH_FAILED = 0,  // operator ran but rejected the operand
    DSD_MATH_OK     = 1,  // operator produced a new value
};

// Which unary operator to apply.
enum dsDATA_UNARY_OP {
    DSD_UNARY_NEG   = 0, // arithmetic negation
    DSD_UNARY_NOT   = 1, // logical/bitwise not
    _DSD_UNARY_USER = 2, // first user-defined operator slot
    _DSD_UNARY_MAX  = 8, // operator-table size
};

// Which binary operator to apply (dsDATA::Math binary overload). DB-verified values
// (types_enum_values dsDATA_BINARY_OP).
enum dsDATA_BINARY_OP {
    DSD_BINARY_ADD     = 0,
    DSD_BINARY_SUB     = 1,
    DSD_BINARY_MUL     = 2,
    DSD_BINARY_DIV     = 3,
    DSD_BINARY_AND     = 4,
    DSD_BINARY_OR      = 5,
    DSD_BINARY_XOR     = 6,
    DSD_BINARY_LOG_AND = 7,
    DSD_BINARY_LOG_OR  = 8,
    DSD_BINARY_EQ      = 9,
    DSD_BINARY_LT      = 10,
    DSD_BINARY_GT      = 11,
    DSD_BINARY_LE      = 12,
    DSD_BINARY_GE      = 13,
    DSD_BINARY_NE      = 14,
    _DSD_BINARY_USER   = 15, // first user-defined operator slot
    _DSD_BINARY_MAX    = 24, // operator-table size
};
