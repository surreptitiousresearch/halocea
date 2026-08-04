#pragma once

/* hkSingleton<T> — empty CRTP singleton base tag (DB: every instantiation is an empty struct,
 * sizeof 0x1; used only as a zero-size base class). Static instance pointers live as
 * hkSingleton<T>::s_instance globals. */
template<class T>
struct hkSingleton
{
};
