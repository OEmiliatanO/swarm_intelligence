#!/bin/bash

mkdir -p plot/
make record all

if [ "$1" == "Ackley" ] || [ "$1" == "1" ]
then
    echo "Ackley"
    mkdir -p plot/Ackley
    ./main.elf --D 2 --k 20000 --initT 30 --rt 0.999 --step 980 --test_function 1 > /dev/null
    python plot.py 1
elif [ "$1" == "Rastrigin" ] || [ "$1" == "2" ]
then 
    echo "Rastrigin"
    mkdir -p plot/Rastrigin
    ./main.elf --D 2 --k 20000 --initT 30 --rt 0.999 --step 980 --test_function 2 > /dev/null
    python plot.py 2
elif [ "$1" == "HappyCat" ] || [ "$1" == "3" ]
then
    echo "HappyCat"
    mkdir -p plot/HappyCat
    ./main.elf --D 2 --k 20000 --initT 30 --rt 0.999 --step 980 --test_function 3 > /dev/null
    python plot.py 3
elif [ "$1" == "Rosenbrock" ] || [ "$1" == "4" ]
then
    echo "Rosenbrock"
    mkdir -p plot/Rosenbrock
    ./main.elf --D 2 --k 20000 --initT 30 --rt 0.999 --step 980 --test_function 4 > /dev/null
    python plot.py 4
elif [ "$1" == "Zakharov" ] || [ "$1" == "5" ]
then
    echo "Zakharov"
    mkdir -p plot/Zakharov
    ./main.elf --D 2 --k 20000 --initT 30 --rt 0.999 --step 980 --test_function 5 > /dev/null
    python plot.py 5
elif [ "$1" == "Michalewicz" ] || [ "$1" == "6" ]
then
    echo "Michalewicz"
    mkdir -p plot/Michalewicz
    ./main.elf --D 2 --k 20000 --initT 30 --rt 0.999 --step 980 --test_function 6 > /dev/null
    python plot.py 6
elif [ "$1" == "all" ] || [ "$1" == "-1" ]
then
    echo "Ackley"
    mkdir -p plot/Ackley
    ./main.elf --D 2 --k 20000 --initT 30 --rt 0.999 --step 980 --test_function 1 > /dev/null
    python plot.py 1
    
    echo "Rastrigin"
    mkdir -p plot/Rastrigin
    ./main.elf --D 2 --k 20000 --initT 30 --rt 0.999 --step 980 --test_function 2 > /dev/null
    python plot.py 2
    
    echo "HappyCat"
    mkdir -p plot/HappyCat
    ./main.elf --D 2 --k 20000 --initT 30 --rt 0.999 --step 980 --test_function 3 > /dev/null
    python plot.py 3
    
    echo "Rosenbrock"
    mkdir -p plot/Rosenbrock
    ./main.elf --D 2 --k 20000 --initT 30 --rt 0.999 --step 980 --test_function 4 > /dev/null
    python plot.py 4
    
    echo "Zakharov"
    mkdir -p plot/Zakharov
    ./main.elf --D 2 --k 20000 --initT 30 --rt 0.999 --step 980 --test_function 5 > /dev/null
    python plot.py 5

    echo "Michalewicz"
    mkdir -p plot/Michalewicz
    ./main.elf --D 2 --k 20000 --initT 30 --rt 0.999 --step 980 --test_function 6 > /dev/null
    python plot.py 6
fi