#!/bin/bash

if [ "$1" == "Ackley" ] || [ "$1" == "1" ]
then
    echo "Ackley"
    ./main.elf --N 1000 --D 2 --k 100 --vmax 0.5 --c1 2 --c2 1 --test_function 1  > Ackley_2D.txt
    ./main.elf --N 1000 --D 10 --k 100 --vmax 0.5 --c1 2.5 --c2 2.5 --test_function 1 > Ackley_10D.txt
    ./main.elf --N 10000 --D 30 --k 100 --vmax 0.5 --c1 2.5 --c2 2.5 --test_function 1 > Ackley_30D.txt
elif [ "$1" == "Rastrigin" ] || [ "$1" == "2" ]
then 
    echo "Rastrigin"
    ./main.elf --N 1000 --D 2 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 2  > Rastrigin_2D.txt
    ./main.elf --N 1000 --D 10 --k 1000 --vmax 0.5 --c1 2 --c2 0.2 --test_function 2 > Rastrigin_10D.txt
    ./main.elf --N 10000 --D 30 --k 5000 --vmax 0.5 --c1 2 --c2 0.2 --test_function 2 > Rastrigin_30D.txt
elif [ "$1" == "HappyCat" ] || [ "$1" == "3" ]
then
    echo "HappyCat"
    ./main.elf --N 1000 --D 2 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 3  > HappyCat_2D.txt
    ./main.elf --N 1000 --D 10 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 3 > HappyCat_10D.txt
    ./main.elf --N 1000 --D 30 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 3 > HappyCat_30D.txt
elif [ "$1" == "Rosenbrock" ] || [ "$1" == "4" ]
then
    echo "Rosenbrock"
    ./main.elf --N 1000 --D 2 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 4  > Rosenbrock_2D.txt
    ./main.elf --N 1000 --D 10 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 4 > Rosenbrock_10D.txt
    ./main.elf --N 1000 --D 30 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 4 > Rosenbrock_30D.txt
elif [ "$1" == "Zakharov" ] || [ "$1" == "5" ]
then
    echo "Zakharov"
    ./main.elf --N 1000 --D 2 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 5  > Zakharov_2D.txt
    ./main.elf --N 1000 --D 10 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 5 > Zakharov_10D.txt
    ./main.elf --N 1000 --D 30 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 5 > Zakharov_30D.txt
elif [ "$1" == "Michalewicz" ] || [ "$1" == "6" ]
then
    echo "Michalewicz"
    ./main.elf --N 1000 --D 2 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 6  > Michalewicz_2D.txt
    ./main.elf --N 1000 --D 10 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 6 > Michalewicz_10D.txt
    ./main.elf --N 1000 --D 30 --k 100 --vmax 1 --c1 2 --c2 0.2 --test_function 6 > Michalewicz_30D.txt
fi
