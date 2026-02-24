# Competitive programming library

C++ library for competitive programming.

## Prerequisites (WSL)

```bash
sudo apt install g++ make clang-format-19
```

## Quick start

```bash
make test
```

## Library

### `cp/core`

| Header       | Description                                                   |
| ------------ | ------------------------------------------------------------- |
| `common.hpp` | `bits/stdc++.h`, namespace cp, type aliases, common constants |

### `cp/ds`

| Header               | Description                                               |
| -------------------- | --------------------------------------------------------- |
| `dsu.hpp`            | Union-find, union by size and path compression            |
| `fenwick.hpp`        | BIT for prefix sums                                       |
| `seg_tree.hpp`       | Segment tree, point update, range query                   |
| `dyn_seg_tree.hpp`   | Lazy segment tree for sparse ranges, range-add, range-sum |
| `range_seg_tree.hpp` | Segment tree, range-add, range-sum                        |

### `cp/math`

| Header        | Description      |
| ------------- | ---------------- |
| `mod_int.hpp` | Modular integers |

## Adding a new module

1. Create `include/cp/<category>/<module_name>.hpp`
2. Add `#pragma once` and wrap everything in `namespace cp { }`
3. Add a corresponding test file at `tests/<category>/test_<module_name>.cpp`
4. Run `make test` to verify

## Conventions

- `PascalCase` for classes and structs
- `snake_case` for functions, methods, variables, parameters, members, and file names
- STL only - no external dependencies
- One concept per header (multiple classes is ok if they are related)
