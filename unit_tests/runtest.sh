#!/bin/bash

cd unit_tests &> /dev/null

if [ $# -lt 1 ]
then
    echo "usage: $0 <unit test, w/o .c>"
    echo -n "available tests: "
    ls *.c

    exit 1
fi

includes="../${1/test/}.c"
if [ "$1" == "dicttest" ]
then
    includes="$includes ../list.c"
fi

gcc -o $1 $1.c $includes && ./$1 &> $1.log
code=$?

if [ $code -ne 0 ]
then
    echo "finished with code: $code"
    exit $code
fi

diff -s -c10 $1.log $1-expected.log