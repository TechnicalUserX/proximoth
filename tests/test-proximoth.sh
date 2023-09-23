#!/bin/bash

# Proximoth Test
# Check Proximoth availability from terminal

if [ ! -f ../build/bin/proximoth ]; then
    echo "Proximoth executable not found"
    exit 1
fi

cd ../build/bin
./proximoth

return_code=$?

if [ ${return_code} -eq 0 ]
then
    echo "Proximoth is compiled successfully"
    exit 0
else
    echo "Proximoth exited with failure code ${return_code}"
    exit ${return_code}
fi