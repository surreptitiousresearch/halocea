#ifndef HS_SCRIPT_TYPE_H
#define HS_SCRIPT_TYPE_H

/* hs_script_type — the scenario script's execution class (hs_script.script_type).
 * Source: DB anonymous enum _46705EB39A95B6A318FAF76EF66BD7AF (types_enum_values, verbatim). */
enum hs_script_type {
	_hs_script_startup = 0,
	_hs_script_dormant = 1,
	_hs_script_continuous = 2,
	_hs_script_static = 3,
	_hs_script_stub = 4,
	NUMBER_OF_HS_SCRIPT_TYPES = 5,
};

#endif /* HS_SCRIPT_TYPE_H */
