#pragma once

/* psGET_FLAGS — the psSECTION Get/GetValueRefs lookup-behavior flags (MASK VALUES, not bit
 * indices — OR/AND them directly). Named by the round-2 flaghex triage from the consumer
 * sites in psSECTION_DATA::GetValueRefs / psSECTION_INTERFACE::GetValue.
 * Source: types_enum_values $6B081ED5C12D703B07A7B25D3D7CA55D (DB-verified, members
 * verbatim in DB order; 8 is named twice — GET and ITER spellings — both kept). */
enum psGET_FLAGS {
	PS_GET_NO_THIS = 1,
	PS_GET_NO_PARENT = 2,
	PS_GET_NO_OWNER = 4,
	PS_GET_SUBSTR = 8,
	PS_ITER_SUBSTR = 8,
	_PS_GET_NO_THIS_SYS = 1073741824,
	_PS_ITER_NO_NAME = -2147483648,
};

// psGET_FLAGS: DB-verified via types_enum_values $6B081ED5C12D703B07A7B25D3D7CA55D
