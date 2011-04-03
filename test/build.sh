#!/bin/bash -x
rm testre2 &>/dev/null
g++ testre2.cc -lre2 -lpthread -o testre2 && ./testre2
