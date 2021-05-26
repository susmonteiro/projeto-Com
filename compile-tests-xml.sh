#!/bin/zsh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

total=0
passed=0

make || exit 1

cd tests
for f in *.fir; do
    total=$(($total+1))

    if ../fir $f --target xml; then
        echo "$f ${GREEN}passed${NC}"
        passed=$((passed+1))
    else
        echo "$f ${RED}compilation error${NC}"
    fi

    # rm "${f%.*}.xml"
done

echo "Tests passed: $passed/$total"