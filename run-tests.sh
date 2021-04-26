#!/bin/sh

total=0
passed=0

cd tests
for f in *.fir; do
    total=$(($total+1))
    
    ../fir --target asm $f || (echo "$f compilation error" ; continue)
    yasm -felf32 "${f%.*}.asm" || (echo "$f compilation error" ; continue)
    ld -melf_i386 -o test_fir  "${f%.*}.o" -lrts || (echo "$f compilation error" ; continue)
    rm "${f%.*}.o"

    ./test_fir > "${f%.*}.out" || (echo "$f execution error" ; continue)

    [ -z "$(diff "${f%.*}.out" "expected/${f%.*}.out")" ] && (echo "$f passed"; passed=$(($passed+1))) || echo "$f failed"

    rm "${f%.*}.out"
    rm test_fir
done

echo "Tests passed $passed/$total"