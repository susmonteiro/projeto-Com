#!/bin/zsh

make &> /dev/null || exit 1

cp $1 .
f=$(basename $1)

./fir -g --target asm $f
cat ${f%.*}.asm | grep -P "^;" | cut -c3-

rm "$f" &> /dev/null
rm "${f%.*}.asm" &> /dev/null