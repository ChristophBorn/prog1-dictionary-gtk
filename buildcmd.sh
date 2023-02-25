#!/bin/bash

gcc *.c -o beleg -Wall $(pkg-config --cflags --libs gtk+-3.0) -export-dynamic
exit $?