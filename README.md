# cp - Competitive Programming Library

A header-only C++ library for competitive programming.

## Prerequisites (WSL)

```bash
sudo apt install g++ make clang-format
```

## Quick start

```bash
make test
```

## Adding a new module

1. Create `include/cp/<category>/<module_name>.hpp`
2. Add `#pragma once` and wrap everything in `namespace cp { }`
3. Add a corresponding test file at `tests/<category>/test_<module_name>.cpp`
4. Run `make test` to verify

## Conventions

- `PascalCase` for classes and structs
- `snake_case` for functions, methods, variables, parameters, members, and file names
- STL only - no external dependencies
- One class/concept per header
- Every public function must document time and space complexity
