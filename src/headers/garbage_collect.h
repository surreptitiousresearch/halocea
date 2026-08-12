#pragma once

/* garbage_collect — the collection mode objects_garbage_collection picks from the current pressure
 * on object_memory_pool and the object datum array, and which then selects that pass's stopping
 * target: _everything is the forced pass (object_globals->force_garbage_collection, drains the whole
 * garbage list), _active_objects is taken when the active garbage-object count reaches 50 and stops
 * once it is back under 30, and _for_space is taken when pool space or datum slots run low and stops
 * once both recover (it also runs the up-front memory_pool_compact and owns the "garbage collection
 * critical" console warning). The mode local's -1 ("nothing to do") is the locked raw sentinel and is
 * NOT a member of this family. Source: types_enum_values $2C26E3BCB6D1062BA7AE0B472422981A
 * (DB-verified). */
enum garbage_collect {
	_garbage_collect_everything = 0,
	_garbage_collect_active_objects = 1,
	_garbage_collect_for_space = 2,
	NUMBER_OF_GARBAGE_COLLECTION_MODES = 3,
};

// garbage_collect: DB-verified via types_enum_values $2C26E3BCB6D1062BA7AE0B472422981A
