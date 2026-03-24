# AGENTS.md

Practical guide for autonomous coding agents in this repository.

## Project Summary

- Project: `HomeAccounts`
- Languages: C and C++
- C++ standard: C++20 (`target_compile_features(... cxx_std_20)`)
- Build system: CMake
- Testing: doctest, discovered via CTest
- App type: wxWidgets desktop GUI (`HomeAccounts` target)

## Repository Layout

- `src/csv`: C parsing and primitive value utilities
- `src/file`: C++ storage abstractions and crypto-backed stores
- `src/gui`: wxWidgets app, documents/views, panels, resources
- `test/csv`: unit tests for csv C modules
- `test/file`: unit tests for storage/file modules
- `test/gui`: tests for GUI-adjacent document/algorithm logic
- `test/wx`: tests for wx helper behavior
- `cmake-common/functions.cmake`: shared CMake helpers and defaults
- `.clang-format` and `.editorconfig`: formatting authority

## Toolchain and Dependencies

- CMake >= 3.21
- Compiler with C++20 support
- Crypto++
- wxWidgets components: `core`, `base`, `xml`, `xrc`, `adv`
- Gettext tools (`msgfmt` used by GUI build)

## Build Commands

### Configure debug

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
```

### Configure release

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

### Build all

```bash
cmake --build build --config Debug
```

### Build specific target

```bash
cmake --build build --target test_csv_parser
cmake --build build --target TestStores
cmake --build build --target HomeAccounts
```

## Test Commands (Including Single Test)

CTest names are generated as:

- `<target> - test:<doctest-case-name>`

### Run all tests

```bash
ctest --test-dir build/test -C Debug --output-on-failure
```

### Run one test executable (all cases in one target)

```bash
ctest --test-dir build/test -C Debug -R '^test_csv_parser - test:'
ctest --test-dir build/test -C Debug -R '^TestStores - test:'
```

### Run one single doctest case through CTest

```bash
ctest --test-dir build/test -C Debug -R '^test_csv_parser - test:parse_line$'
```

### Run one single doctest case directly from binary

```bash
./build/test/csv/test_csv_parser --test-case=parse_line
```

### List tests before writing regex

```bash
ctest --test-dir build/test -C Debug -N
```

### Re-run only failures

```bash
ctest --test-dir build/test -C Debug --rerun-failed --output-on-failure
```

## Linting and Static Gates

- No dedicated lint target exists in CMake.
- Compiler warnings are strict and fatal in `cmake-common/functions.cmake`:
  - `-Werror -Wall -Wextra`
- Treat successful compile as a quality gate.

## Formatting Commands

Use repository clang-format config on touched files only:

```bash
clang-format -i path/to/file.cpp
```

## Style Source Precedence

When rules conflict, use this order:

1. Existing local style in touched module
2. `.clang-format`
3. `.editorconfig`
4. `.github/instructions/*.md`

## Concrete Formatting Rules

- Indentation: 4 spaces
- Tabs: disallowed except makefiles
- Max line length: 120
- Charset: UTF-8
- End-of-line: LF
- Final newline at EOF: required
- C/C++ brace style: Linux style (brace on next line)
- Avoid long blank-line runs

## Includes and Headers

- Use include guards (`_HA_<AREA>_<NAME>_H_` style)
- Separate system/third-party includes from local includes with one blank line
- Keep headers lightweight; move heavy includes into `.cpp` where practical
- In test files, include `doctest/doctest.h` first

## Naming Conventions

- C++ classes/types: `PascalCase`
- C++ methods/functions: `camelCase`
- C functions/helpers: `snake_case`
- Member fields: `m_` prefix
- Macros/constants: `UPPER_CASE`
- Test files follow existing mixed style (`test_*.cpp` and `Test*.cpp`)

## Types and API Boundaries

- Preserve `extern "C"` boundaries for C interfaces
- Keep API-compatible integer types (`int64_t`, `size_t`) where already used
- Prefer `explicit` for conversion-sensitive single-argument constructors
- Match existing module architecture; avoid unrequested wide refactors

## Error Handling Guidelines

- Storage/file layer uses exceptions (`std::runtime_error` subclasses)
- GUI layer can log recoverable failures with wx logging (`wxLogError`)
- Do not silently ignore errors; throw, return, or log with context

## Ownership and Memory

- Codebase currently mixes raw pointers and RAII patterns
- In scoped edits, follow local ownership pattern for consistency
- Keep ownership explicit and avoid introducing unclear sharing

## Testing Style Guidelines

- Use doctest `TEST_CASE` and `SUBCASE`
- Prefer deterministic, precise checks (`CHECK(...)`)
- Clean temporary test artifacts (files/directories)
- For bug fixes, add or update the narrowest related test when feasible

## Checked-in Agent Instruction Files

Repository contains:

- `.github/instructions/cpp.instructions.md`
- `.github/instructions/coding-style.instructions copy.md`

Rules extracted from those files:

- Follow `.clang-format` and `.editorconfig`
- Keep comments concise
- Preferred comment tone: lowercase start, no ending punctuation

## Cursor / Copilot Rules Status

Checked for Cursor rule files:

- `.cursorrules`: not found
- `.cursor/rules/`: not found

Checked for Copilot rule file:

- `.github/copilot-instructions.md`: not found

If any of these files appear later, treat them as mandatory policy.

## Recommended Agent Workflow

1. Read nearby code before editing.
2. Keep changes minimal and scoped.
3. Build affected targets first.
4. Run the smallest relevant test subset (single test preferred).
5. Run broader tests only if scope justifies it.

## Implement a Feature

Read file `spec/features.md` to find the detail description of the feature if only a number is mentioned for the feature.

## Fix a Bug

Read file `spec/features.md` to find the detail description of the bug if only a number is mentioned for the bug.
