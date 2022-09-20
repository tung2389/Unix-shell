#! /bin/bash

if ! [[ -x dush ]]; then
    echo "dush executable does not exist"
    exit 1
fi

./tester/run-tests.sh $*