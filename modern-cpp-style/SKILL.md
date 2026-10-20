---
name: modern-cpp-style
description: House style for writing and reviewing Modern C++ (C++17/20/23) — brace initialization, const/constexpr by default, static_assert, enum class with unary operator+, snake_case functions, trailing-underscore members, STL algorithms/ranges over raw loops, RAII, explicit constructors. Use this skill whenever the user asks to write, generate, refactor, fix, or review C++ code (.cpp, .hpp, .h, .cc files, CMake projects, snippets pasted in chat), even if they don't mention style or conventions.
---

# Modern C++ house style

You are acting as an expert in Modern C++ (C++17/20/23) and clean code. Generate reliable, safe, high-performance code and review code against the rules below. The rules exist to make code safer (fewer implicit conversions, no UB from narrowing or signed/unsigned mixing), cheaper (no needless allocations), and uniform enough that a reader never wonders which convention a file follows.

A complete, compiling example of the style is in `references/reference_example.cpp`. Read it the first time you use this skill in a conversation if you need a concrete anchor.

## 1. General rules and code review

- **Language.** Code is C++. Every code comment and every explanation of a defect inside code must be in **English**, regardless of the conversation language. Prose outside code blocks follows the user's language.
- **Concrete fixes, not just findings.** Whenever you point out a defect (missing comment, possible UB, missing `const`, wrong naming, raw loop…), immediately give the corrected code fragment or the exact text of the missing comment. A finding without a fix is incomplete.
- **Use modern features and idioms** where they genuinely help: `std::string_view`, `std::optional`, `auto`, `constexpr`, `enum class`, RAII, `static_assert`, `std::span`, ranges.

### Review output format
When reviewing, group findings by severity, and for each finding give: location → problem (one line, English) → fixed code.

~~~
### Critical (UB / leaks / correctness)
1. `parse()` line 12 — `buf.size() - 1` underflows when `buf` is empty.
   ```cpp
   for (int i{}; i < std::ssize(buf) - 1; ++i) { ... }
   ```
### Style
...
~~~
Finish with the full corrected version of the code when it is short enough to be useful (roughly under 150 lines); otherwise give the corrected fragments only.

## 2. Variables, initialization, types

**Initialization**
- Use brace (direct-list) initialization `{}` for all variables and class members — it rejects narrowing conversions at compile time.
- Value-initialize fundamental and trivial types with empty braces: `int count{}`, `double ratio{}`. For a later explicit reset to zero, plain assignment is fine: `count = 0;`.
- For other POD/aggregate types, reset with empty `{}` instead of `{ 0 }` or `= 0` (`{ 0 }` only zeroes the first member explicitly and reads as if that were intended).

**const / constexpr**
- `const` by default on local variables and on member functions that do not change observable state.
- Do **not** add `const` to by-value function parameters (it is noise in the interface). Use `const` on reference and pointer parameters when it expresses that the referenced data is not modified.
- Use `constexpr` (or `consteval` when it must be compile-time) for values and functions computable at compile time.

**static_assert**
- Validate types, struct sizes, template parameters, and constant conditions at compile time, e.g. `static_assert(sizeof(Header) == 16, "Header must match the wire format.");`, `static_assert(std::is_trivially_copyable_v<T>);`.

**Numeric types**
- Prefer plain signed `int` for arithmetic, counters, and indices. Don't reach for `std::ptrdiff_t`, `std::int32_t`, `ssize_t` without a concrete reason (wire formats, hardware registers, large sizes).
- Avoid `unsigned` in loops and ordinary arithmetic — wraparound and signed/unsigned comparison bugs. Use unsigned only for bit masks or when modular arithmetic is the spec. When a container size is needed as a signed value, use `std::ssize(c)` (C++20).

**Strings**
- Read-only string parameters take `std::string_view`, not `const std::string&`, to avoid allocations when callers pass literals or substrings. Mind lifetime: don't store a `string_view` member that outlives its source; store `std::string` instead.

## 3. Naming and formatting

| Entity | Convention | Example |
|---|---|---|
| Classes, structs, type aliases, enums | `PascalCase` | `AdcClippingDetector`, `ClippingReport` |
| Enumerators | `PascalCase` | `CallPreamble::Header` |
| Local variables, parameters | `snake_case` | `iq_samples`, `target_idx` |
| Public functions and methods | `snake_case` | `process_data()`, `get_id()` |
| Private/protected data members | `snake_case_` | `sample_rate_rx_`, `id_` |
| Private/protected helper methods | `snake_case_()` | `log_error_()`, `calculate_internal_()` |

- **Never use leading underscores** (`_var`, `__var`). Names with a double underscore anywhere, or starting with an underscore followed by an uppercase letter, are reserved by ISO C++ for the implementation. A trailing underscore is collision-free.
- **Scoped enums only.** Always `enum class`, never C-style `enum`.
- **Unary `operator+` for enums used as integers.** If an `enum class` is used as an array index, bit flag, counter, or passed to system APIs, provide:
  ```cpp
  [[nodiscard]] constexpr auto operator+(EnumType value) noexcept {
      return static_cast<std::underlying_type_t<EnumType>>(value);
  }
  ```
  Then write `table[+Channel::Rx]` instead of scattering `static_cast`s. In C++23 this is equivalent to `std::to_underlying`, but `+` stays shorter at call sites.
- **Compact formatting.** Don't spread short parameter lists, initializer lists, or simple expressions over several lines without need. Opening brace of functions and methods goes on the same line as the declaration:
  ```cpp
  void process_data(const Data& data) {
      // ...
  }
  ```

## 4. Control flow and algorithms

- Prefer `<algorithm>` / `<numeric>` (`std::all_of`, `std::find_if`, `std::transform`, `std::accumulate`) or C++20 ranges (`std::ranges::any_of`, views) over raw `for`/`while` loops where they express intent more clearly.
- When a loop is genuinely clearer (multiple outputs, early exits with side effects), use range-based `for (const auto& item : collection)`. Don't contort code into an algorithm just to avoid a loop.
- Never use C-style casts `(type)val`. Use `static_cast`, `const_cast`, `reinterpret_cast` (and `std::bit_cast` for type punning) so each conversion's intent is visible and greppable.

## 5. Classes, resources, memory

- **RAII.** No owning raw pointers, no manual `new`/`delete`. Use `std::unique_ptr` (default) or `std::shared_ptr` (only for real shared ownership), created with `std::make_unique` / `std::make_shared`. Non-owning raw pointers/references are fine.
- **Parameter passing.** Heavy objects by `const T&`; `std::string_view` / `std::span` for read-only views; by value + `std::move` when the function takes ownership (sink parameters). Use `std::move` to transfer ownership; don't `std::move` a `const` object or a returned local (it blocks NRVO).
- **`explicit` constructors.** Every constructor callable with a single argument (including ones with defaulted extra parameters) is `explicit`, to prevent implicit conversions.
- **Initialization of members.** Default member initializers in the class body for defaults (`int id_{};`), member initializer lists in constructors for values passed in — never assignment in the constructor body.
- Mark getters and pure functions `[[nodiscard]]` and `noexcept` where appropriate.

## Self-check before answering

Before sending generated code, scan it once against this list:
- brace init everywhere; empty `{}` for zero values
- `const` locals/methods; no `const` on by-value params
- names: PascalCase types, snake_case functions/vars, trailing `_` on private members/helpers, no leading `_`
- `enum class` (+ `operator+` if used as integer)
- no raw owning pointers, no C-style casts, no needless `unsigned`
- single-arg constructors `explicit`
- all comments in English
- every required `#include` present (don't rely on transitive includes, e.g. `<string>`)

If you have a shell with a compiler, compile non-trivial generated code (`g++ -std=c++20 -Wall -Wextra -Wpedantic -fsyntax-only`) before presenting it.
