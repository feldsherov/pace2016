#!/usr/bin/env bash

if [ ! -d ../var ];
then
    mkdir ../var
fi

cd ../var

if [ ! -f ../instances/$1.graph ];
then
    echo There is not instance $1.graph
    exit 1
fi

if [ ! -f ../src/fsolve -o ! -f ../src/fsolve ];
then
    echo You need to comple fsolve and fcheck
    exit 2
fi



if [ ! -f fsolve ];
then
   cp ../src/fsolve .
fi

if [ ! -f fcheck ];
then
   cp ../src/fcheck .
fi

./fsolve < ../instances/$1.graph > $1.ans

if [ ! ./fsolve < ../instances/$1.graph > $1.ans ];
then
    echo FAIL $1
    echo fsolve returns not 0
    exit -2
fi

if ./fcheck ../instances/$1.graph $1.ans;
then
    echo OK $1
    exit 0
else
    echo FAIL $1
    echo fcheck returns not 0
    exit -1
fi
