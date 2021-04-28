#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

total=0
passed=0

cd tests
for f in *.fir; do
    total=$(($total+1))
    ../fir $f --target xml && (echo "$f ${GREEN}passed${NC}") && passed=$(($passed+1)) || (echo "$f ${RED}compilation error${NC}"; continue)
done

echo "Tests passed: $passed/$total"