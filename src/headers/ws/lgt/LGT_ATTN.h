#pragma once

/* LGT_ATTN — lgtLIGHT::atten, the distance-attenuation curve selector.
 * Source: types_enum_values $D2F8DE133C35EE04015360D80BA0CB4B (DB-verified, members verbatim
 * in DB order). Consumer: src/ws/lgt/lgtLIGHT__CalcAttenuation.cpp. */
enum LGT_ATTN {
	LGT_ATTN_NO = 0,
	LGT_ATTN_LINEAR = 1,
	LGT_ATTN_QADRATIC = 2,
	LGT_ATTN_CUBIC = 3,
	LGT_ATTN_SHADER = 4,
	LGT_ATTN_SHADER_FAST = 5,
};

// LGT_ATTN: DB-verified via types_enum_values $D2F8DE133C35EE04015360D80BA0CB4B
