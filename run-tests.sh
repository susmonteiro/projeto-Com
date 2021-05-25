#!/bin/sh

total=0
passed=0
compiled=0

make || exit 1

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

cd tests
for f in *.fir; do
    total=$(($total+1))

    if ../fir --target asm $f && yasm -felf32 "${f%.*}.asm" && ld -melf_i386 -o test_fir "${f%.*}.o" -lrts ; then
    	compiled=$(($compiled+1))
	    timeout 1 ./test_fir > "${f%.*}.out"
        #if ! ./test_fir > "${f%.*}.out" ; then
        #    echo "$f execution error"
        if [ -z "$(diff -wBb <(tr -d '\n' < "${f%.*}.out") <(tr -d '\n' < "expected/${f%.*}.out") || echo xixi)" ]; then
            echo "$f ${GREEN}passed${NC}"
            passed=$(($passed+1))
        else
	        echo "$f ${RED}failed${NC}"
        fi

        rm "${f%.*}.out" &> /dev/null
        rm test_fir &> /dev/null
    else
        echo "$f ${RED}compilation error${NC}"
    fi

    rm "${f%.*}.o" &> /dev/null
    rm "${f%.*}.asm" &> /dev/null
done

echo "Tests compiled: $compiled/$total"
echo "Tests passed: $passed/$total"
