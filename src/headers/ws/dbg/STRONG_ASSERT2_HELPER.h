#pragma once
// ws-engine debug: the Saber "strong assertion" value-capturing helper. STRONG_ASSERT2_HELPER is
// a stateless helper class (no data members) whose static template `asserd` overloads format the
// failed condition plus one, two, or three named operand values and route them to the crash
// reporter. Canonical home so the several *_boundary.h headers that used to each redefine it can
// share one definition (avoids C2011 when two are co-included).
struct STRONG_ASSERT2_HELPER {
    // ??$asserd@...@STRONG_ASSERT2_HELPER@@SA... — single named-value form.
    template<typename T>
    static void asserd(const char *expr, const char *file, int line,
                       const char *valueName, const T &value); // boundary

    // Two-value form (mangle asserd<T1,T2>).
    template<typename T1, typename T2>
    static void asserd(const char *expr, const char *file, int line,
                       const char *name1, const T1 &value1,
                       const char *name2, const T2 &value2); // boundary

    // Three-value form (mangle asserd<T1,T2,T3>).
    template<typename T1, typename T2, typename T3>
    static void asserd(const char *expr, const char *file, int line,
                       const char *name1, const T1 &value1,
                       const char *name2, const T2 &value2,
                       const char *name3, const T3 &value3); // boundary
};
