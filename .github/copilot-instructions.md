# HomeAccounts Copilot Instructions

## Build, test, and formatting commands

- Configure a debug build with `cmake -B build -DCMAKE_BUILD_TYPE=Debug`
- Build everything with `cmake --build build --config Debug`
- Build a single target with `cmake --build build --target test_csv_parser`, `cmake --build build --target TestStores`, or `cmake --build build --target HomeAccounts`
- Run the full test suite with `ctest --test-dir build/test -C Debug --output-on-failure`
- List discovered doctest cases with `ctest --test-dir build/test -C Debug -N`
- Run all cases from one test executable with `ctest --test-dir build/test -C Debug -R '^test_csv_parser - test:'`
- Run one discovered doctest case with `ctest --test-dir build/test -C Debug -R '^test_csv_parser - test:parse_line$'`
- Run one case directly from a test binary with `./build/test/csv/test_csv_parser --test-case=parse_line`
- There is no dedicated lint target; the compile is the lint gate because `cmake-common/functions.cmake` enables `-Werror -Wall -Wextra`
- Format touched C/C++ files with `clang-format -i path/to/file.cpp`

## High-level architecture

- The codebase is split into three main layers:
  - `src/csv`: a C parsing/value library for CSV records, dates, money, ints, strings, and list primitives
  - `src/file`: a C++ storage layer with `Store`, `Cache`, crypto helpers, and concrete backends such as `Sqlite3Store`
  - `src/gui`: the wxWidgets desktop application, linked against both `csv` and `file`
- The app does **not** persist typed business objects directly. `HaDocument` owns a `Cache` of named text sections, and feature data is stored in section names such as `accounts`, `data/<year>`, `import`, and `conf/import_col_map`
- Normal file open/save goes through `Sqlite3Store`, which stores sections in an encrypted SQLite table `files(name, key, data)`. `Cache` loads section names first, reads content lazily, and flushes only dirty sections
- The GUI uses wxWidgets doc/view:
  - `HaApp` registers one `wxDocTemplate` for `HaDocument` and `HaView`
  - `HaMainFrame` owns the main notebook and loads menus/resources from XRC
  - `HaView` creates notebook pages for `Data`, `Accounts`, and `Raw`, and adds `Import` lazily
- Each feature folder under `src/gui` follows the same pattern: a typed document plus table/grid/panel UI wrappers. The common CRTP helpers are `HaCsvTemplate`, `HaTableTemplate`, and `HaGridTemplate`
- `AccountsDoc` holds account metadata and provides ID/name mappings used by `DataDoc`
- `DataPanel` edits one year at a time by loading and saving `data/<year>` sections; `DataDoc` recalculates balances/statistics and preserves date/time ordering
- `ImportPanel` loads an external CSV into the `import` section, stores column mappings in `conf/import_col_map`, and merges imported rows into yearly `DataDoc` instances grouped by record year
- `RawPanel` is a direct section editor: slash-delimited section names are rendered as a tree hierarchy and edits write back to raw sections
- Tests are organized by concern:
  - `test/csv` for parser/value primitives
  - `test/file` for cache, crypto, and storage backends
  - `test/gui` for non-visual GUI/domain logic
  - `test/std` for generic helpers
  - `test/wx` for wx-specific behavior

## Key repository conventions

- Prefer changes that preserve the section-based storage model. New persisted data should usually be introduced as a new section or section family, then loaded through `HaDocument`, not by bypassing `Cache`
- In `src/gui`, follow the existing feature split of `Doc` / `Table` / `Grid` / `Panel` instead of collapsing logic into one class
- Reuse the existing CSV abstractions for typed record editing. `AccountsDoc`, `DataDoc`, and `ImportDoc` all build on `HaCsvTemplate`; new CSV-backed features should generally follow the same pattern
- Event handling is intentionally delegated along the chain **document -> view -> frame**, and panel-specific menu behavior is routed through the active `HaPanel`
- The app is resource and localization driven: UI strings use `_()`, catalogs are loaded in `HaApp`, and menus/widgets come from XRC resources generated during the GUI build
- For C/C++ files, follow `.clang-format` and `.editorconfig`; include order is repository-specific:
  1. C++ standard headers
  2. C standard headers
  3. framework headers such as wxWidgets
  4. the current file's header
  5. same-directory headers
  6. same-module headers from other directories
  7. project headers from other modules
  8. third-party headers
- Use include guards in the `_HA_<AREA>_<NAME>_H_` style
- In headers, prefer forward declarations unless a full definition is required
- In tests, include `doctest/doctest.h` first
- Unused parameters should use `[[maybe_unused]]`
- Keep comments short, start with lowercase, and omit trailing punctuation
- When working on build issues, note that the top-level `CMakeLists.txt` injects local include/library paths from `$HOME/workspace/devel` for Debug builds and `$HOME/include` / `$HOME/lib` otherwise
