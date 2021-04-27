#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

total=0
passed=0

cd tests
for f in *.fir; do
    total=$(($total+1))
    ../fir $f --target xml && (echo "$f ${GREEN}passed${NC}") || (echo "$f ${RED}compilation error${NC}"; continue)
    passed=$(($passed+1))
done

echo "Tests passed: $passed/$total"