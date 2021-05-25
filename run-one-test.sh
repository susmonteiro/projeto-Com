#!/bin/sh

make || exit 1

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

cp $1 .
f=$(basename $1)

if ./fir --target asm $f && yasm -felf32 "${f%.*}.asm" && ld -melf_i386 -o test_fir "${f%.*}.o" -lrts ; then
    ./test_fir | tr -d "\n" > "${f%.*}.out"

    DIFF=$(diff -wBb "${f%.*}.out" "tests/expected/${f%.*}.out")

    if [ -z "$DIFF" ]; then
        echo "${GREEN}passed${NC}"
        passed=$(($passed+f))
    else
        echo "${RED}failed${NC}"
        echo "$DIFF"
    fi

    rm test_fir &> /dev/null
else
    echo "$f ${RED}compilation error${NC}"
fi

rm "$f" &> /dev/null
rm "${f%.*}.o" &> /dev/null
rm "${f%.*}.asm" &> /dev/null