# CLAUDE.md - Rules for this repository

## After editing C++ files

Run formatting then tests:

```bash
git diff --name-only --diff-filter=ACM | \
  grep -E '\.(cpp|hpp|h|cc|cxx)$' | \
  xargs -r clang-format-19 -i
make test
```

## Naming conventions

| Kind | Style |
|---|---|
| Class / Struct names | `PascalCase` |
| Function / Method names | `snake_case` |
| Variables / Parameters / Members / Files | `snake_case` |

## Rules

1. **No external dependencies** - STL only.
2. **One class/concept per header file** - keep headers focused.
3. **Complexity docs** - every public function must have a comment noting time and
   space complexity (e.g., `// O(n log n) time, O(n) space`).
4. **All headers** - must start with `#pragma once` and wrap all code in
   `namespace cp { }`.
5. **All includes** - use rooted paths: `#include "cp/..."` (never relative `../`
   from library code).
6. **Namespace** - flat `namespace cp` only; no nested namespaces. `using namespace std`
   is declared inside `namespace cp` in `common.hpp` - no `std::` prefix needed.
7. **Struct member order** - members first, then constructors, then methods.
8. **Braces** - always use `{}` for control flow bodies, even single-line.
9. **Test every module** - add a corresponding file under `tests/<category>/`.
