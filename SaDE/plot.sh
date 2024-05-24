#!/bin/bash

make record all

if [ "$1" == "Ackley" ] || [ "$1" == "1" ]
then
    echo "Ackley"
    mkdir -p plot/Ackley
    ./main.elf --N 1000 --D 2 --k 100 --test_function 1  > /dev/null
    python plot.py 1
elif [ "$1" == "Rastrigin" ] || [ "$1" == "2" ]
then 
    echo "Rastrigin"
    mkdir -p plot/Rastrigin
    ./main.elf --N 1000 --D 2 --k 100 --test_function 2  > /dev/null
    python plot.py 2
elif [ "$1" == "HappyCat" ] || [ "$1" == "3" ]
then
    echo "HappyCat"
    mkdir -p plot/HappyCat
    ./main.elf --N 1000 --D 2 --k 100 --test_function 3  > /dev/null
    python plot.py 3
elif [ "$1" == "Rosenbrock" ] || [ "$1" == "4" ]
then
    echo "Rosenbrock"
    mkdir -p plot/Rosenbrock
    ./main.elf --N 1000 --D 2 --k 100 --test_function 4  > /dev/null
    python plot.py 4
elif [ "$1" == "Zakharov" ] || [ "$1" == "5" ]
then
    echo "Zakharov"
    mkdir -p plot/Zakharov
    ./main.elf --N 1000 --D 2 --k 100 --test_function 5  > /dev/null
    python plot.py 5
elif [ "$1" == "Michalewicz" ] || [ "$1" == "6" ]
then
    echo "Michalewicz"
    mkdir -p plot/Michalewicz
    ./main.elf --N 1000 --D 2 --k 100 --test_function 6  > /dev/null
    python plot.py 6
elif [ "$1" == "Schwefel" ] || [ "$1" == "7" ]
then
    FN="Schwefel"

    echo ${FN}
    mkdir -p plot/${FN}
    ./main.elf --N 1000 --D 2 --k 100 --test_function 7  > /dev/null
    python plot.py 7
elif [ "$1" == "BentCigar" ] || [ "$1" == "8" ]
then
    FN="BentCigar"

    echo ${FN}
    mkdir -p plot/${FN}
    ./main.elf --N 1000 --D 2 --k 100 --test_function 8  > /dev/null
    python plot.py 8
elif [ "$1" == "DropWave" ] || [ "$1" == "9" ]
then
    FN="DropWave"

    echo ${FN}
    mkdir -p plot/${FN}
    ./main.elf --N 1000 --D 2 --k 100 --test_function 9  > /dev/null
    python plot.py 9
elif [ "$1" == "Step" ] || [ "$1" == "10" ]
then
    FN="Step"

    echo ${FN}
    mkdir -p plot/${FN}
    ./main.elf --N 1000 --D 2 --k 100 --test_function 10  > /dev/null
    python plot.py 10
elif [ "$1" == "all" ] || [ "$1" == "-1" ]
then
    echo "Ackley"
    mkdir -p plot/Ackley
    ./main.elf --N 1000 --D 2 --k 100 --test_function 1  > /dev/null
    python plot.py 1
    
    echo "Rastrigin"
    mkdir -p plot/Rastrigin
    ./main.elf --N 1000 --D 2 --k 100 --test_function 2  > /dev/null
    python plot.py 2
    
    echo "HappyCat"
    mkdir -p plot/HappyCat
    ./main.elf --N 1000 --D 2 --k 100 --test_function 3  > /dev/null
    python plot.py 3
    
    echo "Rosenbrock"
    mkdir -p plot/Rosenbrock
    ./main.elf --N 1000 --D 2 --k 100 --test_function 4  > /dev/null
    python plot.py 4
    
    echo "Zakharov"
    mkdir -p plot/Zakharov
    ./main.elf --N 1000 --D 2 --k 100 --test_function 5  > /dev/null
    python plot.py 5

    echo "Michalewicz"
    mkdir -p plot/Michalewicz
    ./main.elf --N 1000 --D 2 --k 100 --test_function 6  > /dev/null
    python plot.py 6

    FN="Schwefel"
    echo ${FN}
    mkdir -p plot/${FN}
    ./main.elf --N 1000 --D 2 --k 100 --test_function 7  > /dev/null
    python plot.py 7

    FN="BentCigar"
    echo ${FN}
    mkdir -p plot/${FN}
    ./main.elf --N 1000 --D 2 --k 100 --test_function 8  > /dev/null
    python plot.py 8

    FN="DropWave"
    echo ${FN}
    mkdir -p plot/${FN}
    ./main.elf --N 1000 --D 2 --k 100 --test_function 9  > /dev/null
    python plot.py 9

    FN="Step"
    echo ${FN}
    mkdir -p plot/${FN}
    ./main.elf --N 1000 --D 2 --k 100 --test_function 10  > /dev/null
    python plot.py 10
fi
