#!/usr/bin/env bash

genmc="$GMC"

if [[ -z "$genmc" ]]; then
    genmc="genmc"
fi

"$genmc" --program-entry-function=main gmc_tst.cpp
