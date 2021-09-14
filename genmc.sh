#!/usr/bin/env bash

set -e

genmc="$GMC"

if [[ -z "$genmc" ]]; then
    genmc="genmc"
fi

CFLAGS="-Wno-implicit-function-declaration --std=c17 -DGMC"

TESTS="initial_value inc concurrent_monotonic with_atomic_lb with_atomic_ub"

for test in $TESTS; do
  echo "Model checking test $test"
  "$genmc" --program-entry-function=genmc_"$test" --dump-error-graph=err.dot --sr -- $CFLAGS test.c
done
