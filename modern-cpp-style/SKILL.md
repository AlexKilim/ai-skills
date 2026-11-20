---
name: modern-cpp-style
description: House style for writing and reviewing Modern C++ (C++17/20/23) — brace initialization, const/constexpr by default, static_assert, enum class with unary operator+, snake_case functions, trailing-underscore members, STL algorithms/ranges over raw loops, RAII, explicit constructors. Use this skill whenever the user asks to write, generate, refactor, fix, or review C++ code (.cpp, .hpp, .h, .cc files, CMake projects, snippets pasted in chat), even if they don't mention style or conventions.
---

# Modern C++ house style

You are acting as an expert in Modern C++ (C++17/20/23) and clean code. Generate reliable, safe, high-performance code and review code against the rules below. The rules exist to make code safer (fewer implicit conversions, no UB from narrowing or signed/unsigned mixing), cheaper (no needless allocations), and uniform enough that a reader never wonders which convention a file follows.

See [the C++17 reference example](references/reference_example.cpp) when you need a working style example.

## 1. General rules and code review

- **Target standard.** Check the standard of the target you are changing and use language and library features it supports. For standalone examples, use the intended consumer's standard.
- **Scope.** Start with the existing interface and add an abstraction when the task needs it. Preserve caller contracts outside the requested change. Include adjacent cleanup when the task requires it or your edit created the issue.
- **Language.** Code is C++. Every code comment and every explanation of a defect inside code must be in **English**, regardless of the conversation language. Prose outside code blocks follows the user's language.
- **Concrete fixes, not just findings.** Whenever you point out a defect (missing comment, possible UB, missing `const`, wrong naming, raw loop…), immediately give the corrected code fragment or the exact text of the missing comment. A finding without a fix is incomplete.
- **Use modern features and idioms** where they genuinely help: `std::string_view`, `std::optional`, `auto`, `constexpr`, `enum class`, RAII, `static_assert`, `std::span` and ranges (C++20).

### Review output format
When reviewing, group findings by severity, and for each finding give: location → problem (one line, English) → fixed code.

~~~
### Critical (UB / leaks / correctness)
1. `parse()` line 12 — `buf.size() - 1` underflows when `buf` is empty.
   ```cpp
   for (decltype(buf.size()) i{1}; i < buf.size(); ++i) {
       compare(buf[i - 1], buf[i]);
   }
   ```
### Style
...
~~~
Finish with the full corrected version of the code when it is short enough to be useful (roughly under 150 lines); otherwise give the corrected fragments only.

## 2. Variables, initialization, types

**Initialization**
- Prefer brace initialization to catch narrowing conversions. Use parentheses when the constructor overload matters. For example, `std::vector<int> values(10)` creates ten elements while `std::vector<int> values{10}` creates one element with value 10.
- Value-initialize fundamental and trivial types with empty braces: `int count{}`, `double ratio{}`. For a later explicit reset to zero, plain assignment is fine: `count = 0;`.
- For other POD/aggregate types, reset with empty `{}` instead of `{ 0 }` or `= 0` (`{ 0 }` only zeroes the first member explicitly and reads as if that were intended).

**const / constexpr**
- `const` by default on local variables and on member functions that do not change observable state.
- Do **not** add `const` to by-value function parameters (it is noise in the interface). Use `const` on reference and pointer parameters when it expresses that the referenced data is not modified.
- Use `constexpr` (or `consteval` in C++20 when every call must be evaluated at compile time) for values and functions computable at compile time.

**static_assert**
- Validate types, struct sizes, template parameters, and constant conditions at compile time, e.g. `static_assert(sizeof(Header) == 16, "Header must match the wire format.");`, `static_assert(std::is_trivially_copyable_v<T>);`.

**Domain types.** Use a distinct type when callers can confuse quantities or identifiers with the same representation. Reuse a suitable type such as `std::chrono::milliseconds` before introducing a wrapper. When validity is part of the type's contract, enforce it at construction or through a checked factory.

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

- Use domain terms and name functions after observable effects. Comments explain constraints, rationale, or non-obvious invariants.
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
- Never use C-style casts `(type)val`. Use `static_cast`, `const_cast`, `reinterpret_cast` (and `std::bit_cast` in C++20 for type punning) so each conversion's intent is visible and greppable.

## 5. Interfaces, resources, and errors

- **RAII.** Give every resource one ownership model. A Qt parent owns its child QObjects, so a child created with `new QLabel(parent)` can be observed through a raw pointer. Keep a single owner throughout its lifetime. For resources owned by ordinary C++ code, avoid manual `new`/`delete`. Use `std::unique_ptr` (default) or `std::shared_ptr` (only for real shared ownership), created with `std::make_unique` / `std::make_shared`. Non-owning raw pointers/references are fine.
- **Rule of zero.** Prefer resource-owning members that already implement the required ownership semantics. Let the compiler generate destruction, copy, and move when member behavior matches the class contract.
- **Parameter passing.** Heavy objects by `const T&`; `std::string_view` / `std::span` (C++20) for read-only views; by value + `std::move` when the function takes ownership (sink parameters). Use `std::move` to transfer ownership; don't `std::move` a `const` object or a returned local (it blocks NRVO).
- **Results and modes.** In new interfaces, prefer returning values, including named result structs for multiple outputs. Use caller-provided buffers when required by an existing ABI or documented allocation or latency constraints. Express mode-selecting flags with a named type or separate operations when the call would otherwise be ambiguous.
- **Failure guarantees.** Follow the project's error model. Preserve invariants on failure and define what state the caller can observe. When the contract requires an all-or-nothing update, prepare and validate the new state before committing it.
- **Side effects.** When separating a calculation makes it easier to test, pass its inputs explicitly. For a timeout calculation, the caller can read the clock and pass the current time.
- **`explicit` constructors.** Every constructor callable with a single argument (including ones with defaulted extra parameters) is `explicit`, to prevent implicit conversions.
- **Initialization of members.** Default member initializers in the class body for defaults (`int id_{};`), member initializer lists in constructors for values passed in — never assignment in the constructor body.
- Mark getters and pure functions `[[nodiscard]]` and `noexcept` where appropriate.

## Self-check before answering

Before sending generated code, scan it once against this list:
- initialization selects the intended constructor and value
- `const` locals/methods; no `const` on by-value params
- names: PascalCase types, snake_case functions/vars, trailing `_` on private members/helpers, no leading `_`
- `enum class` (+ `operator+` if used as integer)
- no raw owning pointers, no C-style casts, no needless `unsigned`
- single-arg constructors `explicit`
- all comments in English
- every required `#include` present (don't rely on transitive includes, e.g. `<string>`)

Build nontrivial changes through the project's configured target. Check a standalone snippet with its intended compiler and standard.
