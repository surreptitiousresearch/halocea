#pragma once
// ws-engine vid: fixed-function render-state block of a render pass. DB-verified layout
// (types_members vidPASS_RENDERSTATE, size 32); the four mode fields are 1-byte-stored enums
// (DB member size 1) — declared unsigned char with their DB enum spelled alongside.

struct vidPASS_RENDERSTATE {
    // DB-verified values (types_enum_values vidPASS_RENDERSTATE::Z_FUNC).
    enum Z_FUNC {
        Z_FUNC_LESS_EQUAL    = 0,
        Z_FUNC_EQUAL         = 1,
        Z_FUNC_LESS          = 2,
        Z_FUNC_GREATER_EQUAL = 3,
        Z_FUNC_GREATER       = 4,
    };
    // DB-verified values (types_enum_values vidPASS_RENDERSTATE::CULLMODE).
    enum CULLMODE {
        CULLMODE_NONE      = 0,
        CULLMODE_FRONTFACE = 1,
        CULLMODE_BACKFACE  = 2,
    };
    // DB-verified values (types_enum_values vidPASS_RENDERSTATE::BLEND_RGB).
    enum BLEND_RGB {
        BLEND_RGB_NONE                = 0,
        BLEND_RGB_BLACK               = 1,
        BLEND_RGB_OPAQUE              = 2,
        BLEND_RGB_BLEND               = 3,
        BLEND_RGB_ADD                 = 4,
        BLEND_RGB_ADD_NO_ALPHA        = 5,
        BLEND_RGB_ADD_SMOOTH_NO_ALPHA = 6,
        BLEND_RGB_PREMUL_A            = 7,
        BLEND_RGB_MUL                 = 8,
        BLEND_RGB_MUL_INV             = 9,
        BLEND_RGB_LIGHTEN             = 10,
        BLEND_RGB_DARKEN              = 11,
        BLEND_RGB_SUBTRACT            = 12,
        BLEND_RGB_INVERT              = 13,
    };
    // DB-verified values (types_enum_values vidPASS_RENDERSTATE::BLEND_A).
    enum BLEND_A {
        BLEND_A_NONE         = 0,
        BLEND_A_BLACK        = 1,
        BLEND_A_OPAQUE       = 2,
        BLEND_A_BLEND        = 3,
        BLEND_A_ADD          = 4,
        BLEND_A_ADD_NO_ALPHA = 5,
        BLEND_A_PREMUL_A     = 6,
        BLEND_A_MUL          = 7,
        BLEND_A_MUL_INV      = 8,
    };

    bool          use_fog;         // 0x00
    bool          akill;           // 0x01 alpha-kill enable
    unsigned char _pad02[2];       // 0x02
    int           akill_ref;       // 0x04 alpha-kill reference value
    bool          z_write;         // 0x08
    bool          z_test;          // 0x09
    unsigned char z_func;          // 0x0A Z_FUNC (DB stores as 1 byte)
    unsigned char _pad0B;          // 0x0B
    float         z_bias;          // 0x0C
    float         z_bias_ss;       // 0x10 screen-space z bias
    unsigned char cullmode;        // 0x14 CULLMODE (DB stores as 1 byte)
    bool          color_write_rgb; // 0x15
    bool          color_write_r;   // 0x16
    bool          color_write_g;   // 0x17
    bool          color_write_b;   // 0x18
    bool          color_write_a;   // 0x19
    unsigned char blend_rgb;       // 0x1A BLEND_RGB (DB stores as 1 byte)
    unsigned char blend_a;         // 0x1B BLEND_A (DB stores as 1 byte)
    char          gpr_count_vs;    // 0x1C vertex-shader GPR budget
    char          gpr_count_ps;    // 0x1D pixel-shader GPR budget
    bool          null_ps;         // 0x1E
    bool          gamma;           // 0x1F
};                                 // 32 bytes
