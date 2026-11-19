# cpp-ci

Shared GitHub Actions CI for C++ libraries built with CMake.

- `setup-cxx`: composite action that installs GCC or Clang (Ubuntu) or selects
  an MSVC toolset by version prefix (Windows, via
  [yaito3014/setup-msvc](https://github.com/yaito3014/setup-msvc)), plus Ninja.
- `.github/workflows/cmake-test.yml`: reusable workflow that builds and tests a
  CMake project with Ninja over C++ versions x Debug/Release x
  {GCC 14, Clang 21, Clang 21 + libc++, MSVC 14.44, MSVC 14.5x}, caches
  `build/_deps`, and posts the warning table with
  [cpp-warning-notifier](https://github.com/iris-cpp/cpp-warning-notifier).

## Reusable workflow

```yaml
name: Test

permissions:
  contents: read
  id-token: write

concurrency:
  group: ${{ github.workflow }}-${{ github.ref }}
  cancel-in-progress: true

on:
  push:
    branches: [main]
  pull_request:
    branches: [main]

jobs:
  test:
    uses: yaito3014/cpp-ci/.github/workflows/cmake-test.yml@v0.1.0
    with:
      cxx-versions: "[11, 14, 17, 20, 23, 26]"
      cmake-args: -DMY_PROJECT_CI=ON
```

| Input | Default | |
|---|---|---|
| `cxx-versions` | (required) | JSON array of C++ versions |
| `cmake-args` | `""` | Extra configure arguments, space-separated and unquoted (passed to both bash and pwsh) |
| `source-dir` | `.` | Directory containing the top-level `CMakeLists.txt` |
| `notify` | `true` | Run cpp-warning-notifier |

MSVC is skipped for C++11 (no such mode) and C++26 (CMake has no flag; the
C++23 jobs use `-std:c++latest`). Tests run with `ctest --test-dir build`.

`id-token: write` is required even with `notify: false`, because GitHub checks
the nested job's permissions before evaluating its `if`.

## setup-cxx

```yaml
- id: setup
  uses: yaito3014/cpp-ci/setup-cxx@v0.1.0
  with:
    compiler: gcc        # gcc | clang | msvc
    version: 14          # gcc/clang major version
    # toolset-prefix: "14.44"   # msvc
- run: cmake -G Ninja -DCMAKE_CXX_COMPILER=${{ steps.setup.outputs.cxx }} -B build .
```

Outputs: `cxx` (compiler executable) and `msvc-toolset` (full toolset version,
msvc only).

## Releasing

`cmake-test.yml` pins `setup-cxx` by tag, so a release bumps that reference to
the new tag in the same commit that gets tagged. The `setup-cxx` job in this
repository's own test workflow tests the action at the current commit.
