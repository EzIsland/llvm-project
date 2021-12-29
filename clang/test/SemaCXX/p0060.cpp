// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_OPERATOR_IS_PARSEABLE -Xclang -verify 
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_BASIC -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_LOOKUP_PREFERENCE -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_LOOKUP_PREFERENCE_REQUIRES -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_LOOKUP_PREFERENCE_SFINAE -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_BASIC_INPUT_OUTPUT -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_BASIC_INPUT_OUTPUT_TEMPLATES -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_TYPE_DEDUCTION -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_PERFECT_FORWARDING -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_VALUE_CATEGORY_OF_TARGET -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_OBJECT_TYPE_TRAITS -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_IS_CONSTEXPR_CALLABLE -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_FORWARDS_NOEXCEPT -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_IS_SFINAE_FIENDLY -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_EXPLICIT_OPERATOR_CALL -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_SATISFIES_CONCEPT -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_ERRORS_ON_FIELDS -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_ERRORS_ON_NONTEMPLATED_OPERATOR_DOT -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_EXPLICIT_OPERATOR_OVERLOADS -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_EXPLICIT_CONVERSION_OPERATOR -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_ERRORS_OPERATOR_OVERLOADS -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_ERRORS_CONVERSION_OPERATOR -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_CHAINING -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_PREFERENCE_FOR_OPERATOR_DOT -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_NON_MEMBER_OPERATOR_DOT_ERRORS -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_TEMPLATED_NON_MEMBER_OPERATOR_DOT_ERRORS -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_ON_STATIC_FUNCTIONS_DOT_SYNTAX -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_ON_STATIC_FUNCTIONS_SCOPE_SYNTAX -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_ERRORS_ON_STATIC_OPERATOR_DOT -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_OF_EXPLICIT_MEMBER_FUNCTION_CALL -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_OF_IMPLICIT_MEMBER_FUNCTION_CALL_ERRORS -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_OF_EXPLICIT_ARROW_MEMBER_FUNCTION_CALL_ERRORS -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_OF_ARROW_CALL_ERRORS -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only -fp0060 %s -DINTERCESSION_MUTUAL_RECURSION -Xclang -verify
// RUN: %clang++ -std=c++20 -fsyntax-only %s -DINTERCESSION_FEATURED_OFF_NEGATIVE -Xclang -verify

#include <type_traits>
#include <utility>

#ifdef INTERCESSION_OPERATOR_IS_PARSEABLE
// expected-no-diagnostics
struct Intercession {
  void operator.() { }
};

#elif INTERCESSION_BASIC
// expected-no-diagnostics
struct Foo {
  void foo() { };
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

int main() {
  Intercession inter;
  inter.foo();
}

#elif INTERCESSION_LOOKUP_PREFERENCE
// expected-no-diagnostics
struct CalledTargetFoo { };

struct CalledIntercessionFoo { };

struct Foo {
  CalledTargetFoo foo() { return {}; };
};

struct Intercession {
  CalledIntercessionFoo foo() { return {}; };
  
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo()), CalledIntercessionFoo>);

#elif INTERCESSION_LOOKUP_PREFERENCE_REQUIRES

struct CalledTargetFoo { };

struct CalledIntercessionFoo { };

struct Empty { };

struct Foo {
  CalledTargetFoo foo(Empty) { return {}; };
};

struct Intercession {
  template<typename T>
  CalledIntercessionFoo foo(T t) // expected-note {{candidate template ignored: constraints not satisfied [with T = Empty]}}
    requires(std::is_same_v<T, int>) { }; // expected-note {{because 'std::is_same_v<Empty, int>' evaluated to false}}
  
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(0)), CalledIntercessionFoo>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(Empty{})), CalledTargetFoo>); // expected-error {{no matching member function for call to 'foo'}}

#elif INTERCESSION_LOOKUP_PREFERENCE_SFINAE

struct CalledTargetFoo { };

struct CalledIntercessionFoo { };

struct Foo {
  CalledTargetFoo foo() {
    return {};
  };
};

struct Intercession {
  template<typename T, typename = std::enable_if_t<std::is_same_v<T, int>>>
  CalledIntercessionFoo foo(T t) { }; // expected-note {{candidate template ignored: requirement 'std::is_same_v<Empty, int>' was not satisfied [with T = Empty]}}
  
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};


struct Empty { };

static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(0)), CalledIntercessionFoo>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(Empty{})), CalledTargetFoo>); // expected-error {{no matching member function for call to 'foo'}}

#elif INTERCESSION_BASIC_INPUT_OUTPUT
// expected-no-diagnostics
struct Output { };
struct Input1 { };
struct Input2 { };

struct Foo {
  Output foo(Input1, Input2) {
    return Output{};
  };
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

int main() {
  Intercession inter;
  Output out = inter.foo(Input1{}, Input2{});
}

#elif INTERCESSION_BASIC_INPUT_OUTPUT_TEMPLATES
// expected-no-diagnostics
struct Output { };
struct Input1 { };
struct Input2 { };

struct Foo {
  template<typename Input1T, typename Input2T>
  Output foo(Input1T, Input2T) {
    static_assert(std::is_same_v<Input1T, Input1>);
    static_assert(std::is_same_v<Input2T, Input2>);
    return Output{};
  };
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

int main() {
  Intercession inter;
  Output out = inter.foo(Input1{}, Input2{});
}

#elif INTERCESSION_TYPE_DEDUCTION
// expected-no-diagnostics
struct Foo {
  template<typename T>
  decltype(auto) foo(T&& aValue) {
    return std::forward<T>(aValue);
  };
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

struct Value { };
Value lvalue;
const Value const_lvalue;
volatile Value volatile_lvalue;
const volatile Value const_volatile_lvalue;

static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(Value{})), Value&&>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(static_cast<const Value&&>(Value{}))), const Value&&>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(lvalue)), Value&>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(const_lvalue)), const Value&>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(volatile_lvalue)), volatile Value&>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(const_volatile_lvalue)), const volatile Value&>);

#elif INTERCESSION_PERFECT_FORWARDING
// expected-no-diagnostics
struct RROverload { };
struct RLOverload { };
struct LROverload { };
struct LLOverload { };

struct Value { };

RROverload baz(Value&&, Value&&) { return RROverload{}; }
RLOverload baz(Value&&, Value&) { return RLOverload{}; }
LROverload baz(Value&, Value&&) { return LROverload{}; }
LLOverload baz(Value&, Value&) { return LLOverload{}; }

struct Foo {
  template<typename... Args>
  decltype(auto) foo(Args&&... aArgs) {
    return baz(std::forward<Args>(aArgs)...);
  };
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

Value lvalue;

static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(Value{},Value{})), RROverload>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(Value{},lvalue)), RLOverload>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(lvalue,Value{})), LROverload>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo(lvalue,lvalue)), LLOverload>);

#elif INTERCESSION_VALUE_CATEGORY_OF_TARGET
// expected-no-diagnostics
struct RValueOverload { };
struct LValueOverload { };
struct ConstLValueOverload { };
struct ConstRValueOverload { };

struct Foo {
  template<typename... Args>
  LValueOverload foo(Args&&... aArgs) & {
    return LValueOverload{};
  }
  
  template<typename... Args>
  RValueOverload foo(Args&&... aArgs) && {
    return RValueOverload{};
  }
  
  template<typename... Args>
  ConstLValueOverload foo(Args&&... aArgs) const & {
    return ConstLValueOverload{};
  }
};

struct Intercession {
  Foo fFoo;

  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) & {
        return std::forward<Func>(aFunc)(fFoo, std::forward<Args>(aArgs)...);
  }
  
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) const & {
    return std::forward<Func>(aFunc)(fFoo, std::forward<Args>(aArgs)...);
  }

  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) && {
     return std::forward<Func>(aFunc)(std::move(fFoo), std::forward<Args>(aArgs)...);
  }
};

Intercession lvalue;
const Intercession const_lvalue;

static_assert(std::is_same_v<decltype(Intercession{}.foo()), RValueOverload>);
static_assert(std::is_same_v<decltype(lvalue.foo()), LValueOverload>);
static_assert(std::is_same_v<decltype(const_lvalue.foo()), ConstLValueOverload>);

#elif INTERCESSION_OBJECT_TYPE_TRAITS
// expected-no-diagnostics
// FROM STACK OVERFLOW: 
template<typename Func, int=(Func{}(), 0)>
constexpr bool is_constexpr(Func&& aFunc) {
  return true;
}

constexpr bool is_constexpr(...) {
  return false;
}

struct Foo {
  void foo() { };
};

struct Intercession {
  template<typename Func>
  void operator.(Func&&) {
    static_assert(std::is_rvalue_reference_v<Func&&>);
    
    using T = std::remove_cv_t<std::remove_reference_t<Func>>;
    static_assert(std::is_trivially_constructible_v<T>);
    static_assert(std::is_nothrow_constructible_v<T>);
    static_assert(std::is_trivially_copy_constructible_v<T>);
    static_assert(std::is_nothrow_copy_constructible_v<T>);
    static_assert(std::is_trivially_move_constructible_v<T>);
    static_assert(std::is_nothrow_move_constructible_v<T>);
    static_assert(std::is_trivially_copy_assignable_v<T>);
    static_assert(std::is_nothrow_copy_assignable_v<T>);
    static_assert(std::is_trivially_move_assignable_v<T>);
    static_assert(std::is_nothrow_move_assignable_v<T>);
    static_assert(std::is_trivially_destructible_v<T>);
    static_assert(std::is_nothrow_destructible_v<T>);
    static_assert(std::is_empty_v<T>);
    static_assert(std::is_class_v<T>);
    static_assert(std::is_standard_layout_v<T>);
    static_assert(std::is_trivial_v<T>);
    static_assert(std::is_invocable_v<T&, Foo>);
    static_assert(std::is_invocable_v<const T&, Foo>);
    static_assert(std::is_invocable_v<T&&, Foo>);
    static_assert(std::is_invocable_v<const T&&, Foo>);
    static_assert(is_constexpr([](){ return T{}; }));
    static_assert(is_constexpr([](){ return T{T{}}; }));
    static_assert(is_constexpr([](){ T t; return T{t}; }));
    static_assert(is_constexpr([](){ T t; return t = T{}; }));
    static_assert(is_constexpr([](){ T t; T t2; return t = t2; }));
  }

  int main() {
    Intercession inter;
    inter.foo();
    return 0;
  }
};

#elif INTERCESSION_IS_CONSTEXPR_CALLABLE
// expected-no-diagnostics
struct Foo {
  constexpr int foo() { return 0; }
};

struct Intercession {
  template<typename Func, typename... Args>
  constexpr decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

static_assert(Intercession{}.foo() == 0);

#elif INTERCESSION_FORWARDS_NOEXCEPT
// expected-no-diagnostics
struct Foo {
  void foo() noexcept { }
  void bar() { }
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) noexcept(noexcept(std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...))) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

static_assert(noexcept(Intercession{}.foo()));
static_assert(!noexcept(Intercession{}.bar()));

#elif INTERCESSION_IS_SFINAE_FIENDLY
// expected-no-diagnostics
struct CalledBar { };
struct CalledZap { };

struct Foo {
  CalledBar bar() { return {}; }
  CalledZap zap() { return {}; }
};

template<typename Func>
struct Bar : public Func {
  using Func::operator();

  template<typename T, typename... Args>
  decltype(auto) operator()(T&& t, Args&&... pArgs) requires(!requires() {
      Func{}(std::forward<T>(t), std::forward<Args>(pArgs)...);
     }) {
    return std::forward<T>(t).bar(std::forward<Args>(pArgs)...);
  }
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return Bar<Func>{}(Foo{}, std::forward<Args>(aArgs)...);
  }
};

static_assert(std::is_same_v<decltype(std::declval<Intercession>().foo()), CalledBar>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().blahblah()), CalledBar>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().bar()), CalledBar>);
static_assert(std::is_same_v<decltype(std::declval<Intercession>().zap()), CalledZap>);

int main() {
  Intercession inter;
  inter.foo();
  inter.blahblah();
  inter.bar();
  inter.zap();
}

#elif INTERCESSION_EXPLICIT_OPERATOR_CALL
// expected-no-diagnostics
struct Intercession {
  int operator.(int x) {
    return x;
  }
};

struct IntercessionTemplated {
  template<typename T>
  T operator.(T x) {
    return x;
  }
};

int main() {
  Intercession inter;
  inter.operator.(0);

  IntercessionTemplated interTemplated;
  interTemplated.operator.(0);
  interTemplated.operator.<int>(0);
}
#elif INTERCESSION_SATISFIES_CONCEPT
// expected-no-diagnostics
struct Foo {
  double foo(int, int);
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

template<typename T, typename U>
concept IsSame = std::is_same_v<T, U>;

template<typename T>
concept HasFoo = requires(int x, int y) {
   { T{}.foo(x, y) } -> IsSame<double>;
};

static_assert(HasFoo<Intercession>);

#elif INTERCESSION_ERRORS_ON_FIELDS

struct Foo {
  int foo;
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) { //expected-note {{possible target for call}}
    return std::forward<Func>(aFunc)(Foo{});
  }
};

int main() {
  Intercession inter;
  return inter.foo; //expected-error {{Intercession of member fields via 'operator.' is not supported; did you mean to call it with no arguments?}}
}

#elif INTERCESSION_ERRORS_ON_NONTEMPLATED_OPERATOR_DOT

struct Intercession {
  int operator.(int x) { // expected-note {{'operator.' declared here}}
    return x;
  }
};

int main() {
  Intercession inter{};
  return inter.foo(0); // expected-error {{too many arguments to function call, expected single argument 'x', have 2 arguments}}
}

#elif INTERCESSION_EXPLICIT_OPERATOR_OVERLOADS
// expected-no-diagnostics
struct Foo {
  int operator++() {
    return 0;
  }
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

int main() {
  Intercession inter;
  inter.operator++();
}

#elif INTERCESSION_EXPLICIT_CONVERSION_OPERATOR
// expected-no-diagnostics
struct Foo {
  operator bool() {
    return false;
  }
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

int main() {
  Intercession inter;
  inter.operator bool();
}


#elif INTERCESSION_ERRORS_OPERATOR_OVERLOADS

struct Foo {
  int operator++() {
    return 0;
  }
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

int main() {
  Intercession inter;
  ++inter; //expected-error {{cannot increment value of type 'Intercession'}}
}

#elif INTERCESSION_ERRORS_CONVERSION_OPERATOR
struct Foo {
  operator bool() {
    return false;
  }
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

int main() {
  Intercession inter;
  if(inter) { //expected-error {{value of type 'Intercession' is not contextually convertible to 'bool'}}
    return 0;
  }
  
}


#elif INTERCESSION_CHAINING
// expected-no-diagnostics
struct Foo {
  double foo(int x, int y) {
    return x+y;
  }
};

struct InnerIntercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

struct OuterIntercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(InnerIntercession{}, std::forward<Args>(aArgs)...);
  }
};

int main() {
  OuterIntercession inter;
  inter.foo(0, 0);
}

#elif INTERCESSION_PREFERENCE_FOR_OPERATOR_DOT
struct Foo {
  double operator.(int x, int y) {
    return x + y;
  }
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...); // expected-error {{called object type 'int' is not a function or function pointer}}
  }
};

int main() {
  Intercession inter;
  // Should prefer to call the operator. on Intercession as opposed to on Foo
  inter.operator.(0,0); // expected-note {{in instantiation of function template specialization 'Intercession::operator.<int, int>' requested here}}
}

#elif INTERCESSION_NON_MEMBER_OPERATOR_DOT_ERRORS

struct Foo { };
void operator.(Foo) { } // expected-error {{overloaded 'operator.' must be a non-static member function}}

#elif INTERCESSION_TEMPLATED_NON_MEMBER_OPERATOR_DOT_ERRORS

template<typename T>
void operator.(T) { } // expected-error {{overloaded 'operator.' must be a non-static member function}}

#elif INTERCESSION_ON_STATIC_FUNCTIONS_DOT_SYNTAX
// expected-no-diagnostics
struct Foo {
  static void foo() { }
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

int main() {
  Intercession inter;
  inter.foo();
}

#elif INTERCESSION_ON_STATIC_FUNCTIONS_SCOPE_SYNTAX

struct Foo {
  static void foo() { }
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

int main() {
  Intercession::foo(); //expected-error {{no member named 'foo' in 'Intercession'}}
}

#elif INTERCESSION_ERRORS_ON_STATIC_OPERATOR_DOT

struct Foo { };

struct Intercession {
  template<typename Func, typename... Args>
  static decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) { // expected-error {{overloaded 'operator.' cannot be a static member function}}
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

#elif INTERCESSION_OF_EXPLICIT_MEMBER_FUNCTION_CALL
// expected-no-diagnostics

struct Target {
  void foo() { };
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Target{}, std::forward<Args>(aArgs)...);
  }

  void bar() {
    (*this).foo();
  }
};

#elif INTERCESSION_OF_IMPLICIT_MEMBER_FUNCTION_CALL_ERRORS

struct Target {
  void foo() { };
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Target{}, std::forward<Args>(aArgs)...);
  }

  void bar() {
    foo(); //expected-error {{use of undeclared identifier 'foo'}}
  }
};

#elif INTERCESSION_OF_EXPLICIT_ARROW_MEMBER_FUNCTION_CALL_ERRORS

struct Target {
  void foo() { };
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Target{}, std::forward<Args>(aArgs)...);
  }

  void bar() {
    this->foo(); //expected-error {{no member named 'foo' in 'Intercession'}}
  }
};

#elif INTERCESSION_OF_ARROW_CALL_ERRORS

struct Target {
  void foo() { };
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Target{}, std::forward<Args>(aArgs)...);
  }
};

int main() {
  Intercession* inter = new Intercession;
  (*inter).foo(); // ok
  inter->foo(); // expected-error {{no member named 'foo' in 'Intercession'}}
}

#elif INTERCESSION_MUTUAL_RECURSION
// expected-no-diagnostics

struct Foo {
  template<typename Func, typename... Args>
  int operator.(Func&& aFunc, Args&&... aArgs);
};

struct Bar {
  template<typename Func, typename... Args>
  int operator.(Func&& aFunc, Args&&... aArgs);
};

template<typename Func, typename... Args>
int Foo::operator.(Func&& aFunc, Args&&... aArgs) {
    return std::forward<Func>(aFunc)(Bar{}, std::forward<Args>(aArgs)...);
}

template<typename Func, typename... Args>
int Bar::operator.(Func&& aFunc, Args&&... aArgs) {
  return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
}  

int main() {
  Bar b;
  b.blah(); // This loops infinitely
}

#elif INTERCESSION_FEATURED_OFF_NEGATIVE

struct Foo {
  void foo() { };
};

struct Intercession {
  template<typename Func, typename... Args>
  decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) { // expected-error {{overloaded operator dot not permitted when call intercession is disabled.  Use -fp0060 to enable call intercession.}}
    return std::forward<Func>(aFunc)(Foo{}, std::forward<Args>(aArgs)...);
  }
};

int main() {
  Intercession inter;
  inter.foo(); // expected-error {{no member named 'foo' in 'Intercession'}}
}

#else

static_assert(false, "No Test Point Selected");

#endif

// Until P1985 is accepted, there does not seem to be a natural
// way to std::forward explicit template parameters.
// #ifdef INTERCESSION_FORWARDS_TEMPLATE_PARAMS

// struct Foo {
//   template<typename T, typename U, int X, int Y>
//   void foo() {
//     static_assert(std::is_same_v<T, double>);
//     static_assert(std::is_same_v<U, int>);
//     static_assert(x == 0);
//     static_assert(y == 1);
//   };
// };

// struct Intercession {
//   template<typename... ExplicitTypes, auto... ExplicitNTTPs, typename Func, typename... Args>
//   decltype(auto) operator.(Func&& aFunc, Args&&... aArgs) {
//     return std::forward<Func>(aFunc).operator()<ExplicitTypes..., ExplicitNTTPs...>(Foo{}, std::forward<Args>(aArgs)...);
//   }
// };

// int main() {
//   Intercession inter;
//   inter.foo<double, int, 0, 1>();
// }

// #endif
