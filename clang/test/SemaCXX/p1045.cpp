// RUN: %clang++ -std=c++20 -fsyntax-only %s -DCONSTEXPR_PARAMETER_IS_PARSEABLE -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only %s -DUNNAMED_CONSTEXPR_PARAMETER_IS_PARSEABLE -Xclang -verify

#ifdef CONSTEXPR_PARAMETER_IS_PARSEABLE

// expected-no-diagnostics

void foo(constexpr int x) { }

#elseif UNNAMED_CONSTEXPR_PARAMETER_IS_PARSEABLE

// expected-no-diagnostics

void foo(constexpr int) { }

#else

static_assert(false, "No Test Point Selected");

#endif
