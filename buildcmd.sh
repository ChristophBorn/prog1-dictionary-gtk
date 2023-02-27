#!/bin/bash

commitid=$(git rev-parse --short HEAD 2>/dev/null)
if [ $? -eq 0 ] # git is present
then
    echo -n "$commitid" >.version
    git diff --quiet || echo -n "-dirty" >>.version
fi

gcc *.c -o beleg-prog1 -Wall -D PROGRAMVERSION=\"$(cat .version)\" $(pkg-config --cflags --libs gtk+-3.0) -export-dynamic
exit $?