#!/bin/sh

total=0
passed=0

make || exit 1

cd tests
for f in *.fir; do
    total=$(($total+1))

    if ../fir --target asm $f && yasm -felf32 "${f%.*}.asm" && ld -melf_i386 -o test_fir "${f%.*}.o" -lrts ; then
	./test_fir > "${f%.*}.out"
        #if ! ./test_fir > "${f%.*}.out" ; then
        #    echo "$f execution error"
        if [ -z "$(diff "${f%.*}.out" "expected/${f%.*}.out")" ]; then
            echo "$f passed"
            passed=$(($passed+1))
        else
            echo "$f failed"
        fi

        rm "${f%.*}.out" &> /dev/null
        rm test_fir &> /dev/null
    fi

    rm "${f%.*}.o" &> /dev/null
    rm "${f%.*}.asm" &> /dev/null
done

echo "Tests passed: $passed/$total"
