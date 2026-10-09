#!/bin/sh
set -eu
if [ "${1:-}" = "--check" ]; then
    command -v clang-format >/dev/null 2>&1 || { echo "clang-format is required for --check" >&2; exit 1; }
    find include src platforms tests/unit -type f \( -name '*.c' -o -name '*.h' \) -print0 | xargs -0 clang-format --dry-run --Werror
else
    command -v clang-format >/dev/null 2>&1 || { echo "clang-format is required" >&2; exit 1; }
    find include src platforms tests/unit -type f \( -name '*.c' -o -name '*.h' \) -print0 | xargs -0 clang-format -i
fi
