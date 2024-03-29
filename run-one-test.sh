#!/bin/zsh

make || exit 1

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

cp $1 .
f=$(basename $1)

if ./fir --target asm $f && yasm -felf32 "${f%.*}.asm" && ld -melf_i386 -o test_fir "${f%.*}.o" -lrts ; then
    if ! ./test_fir > "${f%.*}.out" ; then
        echo "$f exited with code 1"
    else
        DIFF=$(diff -wBb <(tr -d '\n' < "${f%.*}.out") <(tr -d '\n' < "tests/expected/${f%.*}.out"))

        if [ -z "$DIFF" ]; then
            echo "${GREEN}passed${NC}"
            passed=$(($passed+1))
        else
            echo "${RED}failed${NC}"
            echo "$DIFF"
        fi
    fi

    rm test_fir &> /dev/null
else
    echo "$f ${RED}compilation error${NC}"
fi

rm "$f" &> /dev/null
rm "${f%.*}.o" &> /dev/null
rm "${f%.*}.asm" &> /dev/null