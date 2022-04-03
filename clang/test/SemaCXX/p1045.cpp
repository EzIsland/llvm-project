// RUN: %clang++ -std=c++20 -fsyntax-only %s -DCONSTEXPR_PARAMETER_IS_PARSEABLE -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only %s -DUNNAMED_CONSTEXPR_PARAMETER_IS_PARSEABLE -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only %s -DDEFAULT_CONSTEXPR_PARAMETER_IS_PARSEABLE -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only %s -DDEFAULT_UNNAMED_CONSTEXPR_PARAMETER_IS_PARSEABLE -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only %s -DCONSTEXPR_PARAMETER_PACK_IS_PARSEABLE -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only %s -DCONSTEXPR_PARAMETER_IS_INVOCABLE -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only %s -DCONSTEXPR_PARAMETER_IN_STATIC_ASSERT_TRUE -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only %s -DCONSTEXPR_PARAMETER_IN_STATIC_ASSERT_FALSE -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only %s -DCONSTEXPR_PARAMETER_IS_NOT_NTTP -Xclang -verify

#ifdef CONSTEXPR_PARAMETER_IS_PARSEABLE

// expected-no-diagnostics

void foo(constexpr int x) { }

#elif UNNAMED_CONSTEXPR_PARAMETER_IS_PARSEABLE

// expected-no-diagnostics

void foo(constexpr int) { }

#elif DEFAULT_CONSTEXPR_PARAMETER_IS_PARSEABLE

// expected-no-diagnostics

void foo(constexpr int x = 0) { }

#elif DEFAULT_UNNAMED_CONSTEXPR_PARAMETER_IS_PARSEABLE

// expected-no-diagnostics

void foo(constexpr int = 0) { }

#elif CONSTEXPR_PARAMETER_PACK_IS_PARSEABLE

// expected-no-diagnostics
void foo(constexpr auto... x) { }

#elif CONSTEXPR_PARAMETER_IS_INVOCABLE

// expected-no-diagnostics

void foo(constexpr int x) { }

void bar() {
  foo(0);
}

#elif CONSTEXPR_PARAMETER_IN_STATIC_ASSERT_TRUE

// expected-no-diagnostics

void foo(constexpr int x) {
  static_assert(x == 0);
}

void bar() {
  foo(0);
}

#elif CONSTEXPR_PARAMETER_IN_STATIC_ASSERT_FALSE

void foo(constexpr int x) {
  static_assert(x == 1); // expected-error {{static_assert failed due to requirement '0 == 1'}}
}

void bar() {
  foo(0); // expected-note {{in instantiation of function template specialization 'foo<0>' requested here}}
}

#elif CONSTEXPR_PARAMETER_IS_NOT_NTTP

void foo(constexpr int x) { }

void bar() {
  foo<0>(); // expected-error
}

#else

static_assert(false, "No Test Point Selected");

#endif
