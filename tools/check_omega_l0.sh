#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT
root=incubator/omega_librarian_l0
cc -std=c11 -O2 -Wall -Wextra -Werror -Wshadow -ffreestanding -fno-builtin -fno-stack-protector -nostdinc -I "$root" -c "$root/omega_gate.c" -o "$tmp/omega.o"
if nm -u "$tmp/omega.o" | grep -q .; then
  echo 'OMEGA_L0_UNDEFINED_IMPORTS=FAIL'
  nm -u "$tmp/omega.o"
  exit 1
fi
echo 'OMEGA_L0_FREESTANDING_ZERO_IMPORTS=PASS_OBJECT_SCOPED'
cc -std=c11 -O2 -Wall -Wextra -Werror -Wshadow -I "$root" "$root/tests/test_omega_gate.c" "$root/omega_gate.c" -o "$tmp/test"
"$tmp/test"
