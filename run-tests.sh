#!/bin/sh

total=0
passed=0

cd tests
for f in *.fir; do
    total=$(($total+1))

    if ../fir --target asm $f && yasm -felf32 "${f%.*}.asm" && ld -melf_i386 -o test_fir "${f%.*}.o" -lrts ; then
        if ! ./test_fir > "${f%.*}.out" ; then
            echo "$f execution error"
        elif [ -z "$(diff "${f%.*}.out" "expected/${f%.*}.out")" ]; then
            echo "$f passed"
            passed=$(($passed+1))
        else
            echo "$f failed"
        fi

        rm "${f%.*}.out"
        rm test_fir
    fi

    rm "${f%.*}.o"
    rm "${f%.*}.asm"
done

echo "Tests passed: $passed/$total"