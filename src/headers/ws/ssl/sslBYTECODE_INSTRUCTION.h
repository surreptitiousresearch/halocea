#pragma once
// ssl subsystem: one compiled SSL-script VM instruction and its opcode set.
// DB-verified layout (types_members sslBYTECODE_INSTRUCTION): cmd@0 (sslBC_CMD),
// anonymous union@4 (int i_param / float f_param) — size 8.
// The opcode enumerator set below is DB-verified verbatim (types_enum_values sslBC_CMD,
// ordinal 1531, 40 values) — it is NOT inferred.

// SSL bytecode opcode. The interpreter (sslSCRIPTED_FUNCTION_BASE::ExecCode) dispatches on this.
enum sslBC_CMD {
    SSL_BC_PUSH_NULL                   = 0,  // push a NULL-typed dsDATA
    SSL_BC_PUSH_INT                    = 1,  // push i_param as int
    SSL_BC_PUSH_BOOL                   = 2,  // push (i_param != 0) as bool
    SSL_BC_PUSH_FLOAT                  = 3,  // push f_param as float
    SSL_BC_PUSH_DSSTRID                = 4,  // push i_param as a dsSTRID
    SSL_BC_PUSH_FUNC_CONST_REF         = 5,  // push a ref into the function const pool (const[i_param])
    SSL_BC_PUSH_FUNC_PARAM_REF         = 6,  // push a ref to param stack slot (paramBase + i_param)
    SSL_BC_PUSH_LOCAL_VAR_REF          = 7,  // push a ref to local var slot (localsBase + i_param)
    SSL_BC_PUSH_STACK_VAL              = 8,  // duplicate operand at depth i_param
    SSL_BC_PUSH_PARENT_REF             = 9,  // push an object-ref to the parent class
    SSL_BC_PUSH_SELF_REF               = 10, // push an object-ref to self (or self.class[i_param])
    SSL_BC_PUSH_LAST_DELAY             = 11, // push the last-delay float
    SSL_BC_POP_VALUE                   = 12, // pop and discard the top operand
    SSL_BC_CONST_NAME_TO_OBJREF        = 13, // resolve const string name -> object ref
    SSL_BC_IDENT_TO_SELF_GLOBAL        = 14, // resolve identifier -> self/global sub-element ref (self-patching, +2 hint slots)
    SSL_BC_IDENT_TO_SELF_GLOBAL_HINT1  = 15, // hint operand for the above (cached class ptr)
    SSL_BC_IDENT_TO_SELF_GLOBAL_HINT2  = 16, // hint operand for the above (cached sub-element idx)
    SSL_BC_GET_SUB_REF                 = 17, // get sub-element ref by const name id (i_param)
    SSL_BC_GET_SUB_REF_FROM_STACK_NAME = 18, // get sub-element ref by name taken off the stack
    SSL_BC_CALL_FUNCTION               = 19, // call function/member; low 16 bits of i_param = argc
    SSL_BC_CALL_FUNCTION_DELAYED       = 20, // schedule a delayed function call
    SSL_BC_RETURN_VALUE                = 21, // pop into retVal and finish
    SSL_BC_RETURN_VOID                 = 22, // set retVal to NULL and finish
    SSL_BC_POP_JUMP_IF_FALSE           = 23, // pop bool; jump to i_param when false
    SSL_BC_POP_JUMP_IF_TRUE            = 24, // pop bool; jump to i_param when true
    SSL_BC_PEEK_JUMP_IF_FALSE          = 25, // peek bool; jump to i_param when false
    SSL_BC_PEEK_JUMP_IF_TRUE           = 26, // peek bool; jump to i_param when true
    SSL_BC_JUMP                        = 27, // unconditional jump to i_param
    SSL_BC_UN_MATH_OP                  = 28, // in-place unary op (i_param = dsDATA_UNARY_OP)
    SSL_BC_BIN_MATH_OP                 = 29, // binary op via MathFromStackToStack (i_param = dsDATA_BINARY_OP)
    SSL_BC_CONVERT_TYPE                = 30, // explicit convert top to usedTypes[i_param]
    SSL_BC_FOREACH_INIT                = 31, // begin a foreach over class-name i_param
    SSL_BC_FOREACH_TERM                = 32, // end the innermost foreach
    SSL_BC_FOREACH_ITERATE             = 33, // advance the foreach cursor, bind loop var i_param
    SSL_BC_RESET_LOCAL_VAR             = 34, // reset local var i_param to a default of a popped type id
    SSL_BC_ASSIGN                      = 35, // assign: SetRefTo the reference under the value
    SSL_BC_NAMED_PARAM                 = 36, // record a named-call-argument name (i_param)
    SSL_BC_DELAYED_ACTION              = 37, // perform a delayed-function action (start/stop/etc.)
    SSL_BC_SUBREF_TO_CONST_NAME        = 38, // convert a sub-element ref on top into its CONST name string
    SSL_BC_LINE_START                  = 39, // source-line marker / debugger breakpoint check (i_param = line)
};

// One compiled instruction: an opcode plus a single immediate operand (int or float view).
typedef struct sslBYTECODE_INSTRUCTION {
    sslBC_CMD cmd;                                 // 0x00 opcode
    // 0x04 immediate (DB anonymous union _782B4F01D0BB9ECDB4C377D45C88B8AC, 4 bytes) — arms
    // injected into struct scope so bodies access i_param / f_param directly.
    union {
        int   i_param;  // 0x00 integer immediate
        float f_param;  // 0x00 float immediate (SSL_BC_PUSH_FLOAT)
    };
} sslBYTECODE_INSTRUCTION;
