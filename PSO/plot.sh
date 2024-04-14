#!/bin/bash

if [ "$1" == "Ackley" ] || [ "$1" == "1" ]
then
    echo "Ackley"
    ./main.elf --N 1000 --D 2 --k 100 --vmax 0.5 --c1 2 --c2 1 --test_function 1  > /dev/null
    python plot.py 1
elif [ "$1" == "Rastrigin" ] || [ "$1" == "2" ]
then 
    echo "Rastrigin"
    ./main.elf --N 1000 --D 2 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 2  > /dev/null
    python plot.py 2
elif [ "$1" == "HappyCat" ] || [ "$1" == "3" ]
then
    echo "HappyCat"
    ./main.elf --N 1000 --D 2 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 3  > /dev/null
    python plot.py 3
elif [ "$1" == "Rosenbrock" ] || [ "$1" == "4" ]
then
    echo "Rosenbrock"
    ./main.elf --N 1000 --D 2 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 4  > /dev/null
    python plot.py 4
elif [ "$1" == "Zakharov" ] || [ "$1" == "5" ]
then
    echo "Zakharov"
    ./main.elf --N 1000 --D 2 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 5  > /dev/null
    python plot.py 5
elif [ "$1" == "Michalewicz" ] || [ "$1" == "6" ]
then
    echo "Michalewicz"
    ./main.elf --N 1000 --D 2 --k 50 --vmax 0.1 --c1 2 --c2 0.2 --test_function 6  > /dev/null
    python plot.py 6
fi
