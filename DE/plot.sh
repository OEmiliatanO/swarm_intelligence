#!/bin/bash

make record all

if [ "$1" == "Ackley" ] || [ "$1" == "1" ]
then
    echo "Ackley"
    ./main.elf --N 1000 --D 2 --k 100 --m 0.5 --c 0.5 --test_function 1  > /dev/null
    python plot.py 1
elif [ "$1" == "Rastrigin" ] || [ "$1" == "2" ]
then 
    echo "Rastrigin"
    ./main.elf --N 1000 --D 2 --k 100 --m 0.5 --c 0.5 --test_function 2  > /dev/null
    python plot.py 2
elif [ "$1" == "HappyCat" ] || [ "$1" == "3" ]
then
    echo "HappyCat"
    ./main.elf --N 1000 --D 2 --k 100 --m 0.5 --c 0.5 --test_function 3  > /dev/null
    python plot.py 3
elif [ "$1" == "Rosenbrock" ] || [ "$1" == "4" ]
then
    echo "Rosenbrock"
    ./main.elf --N 1000 --D 2 --k 100 --m 0.5 --c 0.5 --test_function 4  > /dev/null
    python plot.py 4
elif [ "$1" == "Zakharov" ] || [ "$1" == "5" ]
then
    echo "Zakharov"
    ./main.elf --N 1000 --D 2 --k 100 --m 0.5 --c 0.5 --test_function 5  > /dev/null
    python plot.py 5
elif [ "$1" == "Michalewicz" ] || [ "$1" == "6" ]
then
    echo "Michalewicz"
    ./main.elf --N 1000 --D 2 --k 100 --m 0.5 --c 0.5 --test_function 6  > /dev/null
    python plot.py 6
elif [ "$1" == "all" ] || [ "$1" == "-1" ]
then
    echo "Ackley"
    ./main.elf --N 1000 --D 2 --k 100 --m 0.5 --c 0.5 --test_function 1  > /dev/null
    python plot.py 1
    
    echo "Rastrigin"
    ./main.elf --N 1000 --D 2 --k 100 --m 0.5 --c 0.5 --test_function 2  > /dev/null
    python plot.py 2
    
    echo "HappyCat"
    ./main.elf --N 1000 --D 2 --k 100 --m 0.5 --c 0.5 --test_function 3  > /dev/null
    python plot.py 3
    
    echo "Rosenbrock"
    ./main.elf --N 1000 --D 2 --k 100 --m 0.5 --c 0.5 --test_function 4  > /dev/null
    python plot.py 4
    
    echo "Zakharov"
    ./main.elf --N 1000 --D 2 --k 100 --m 0.5 --c 0.5 --test_function 5  > /dev/null
    python plot.py 5

    echo "Michalewicz"
    ./main.elf --N 1000 --D 2 --k 100 --m 0.5 --c 0.5 --test_function 6  > /dev/null
    python plot.py 6
fi