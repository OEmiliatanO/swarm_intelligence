#!/bin/bash

mkdir -p result/
make dep all clean

if [ "$1" == "Ackley" ] || [ "$1" == "1" ]
then
    echo "Ackley"
    mkdir -p result/Ackley/
    ./main.elf --N 1000 --D 2 --k 500 --test_function 1  > result/Ackley/Ackley_2D.txt
    ./main.elf --N 1000 --D 10 --k 2000 --test_function 1 > result/Ackley/Ackley_10D.txt
    ./main.elf --N 10000 --D 30 --k 3000 --test_function 1 > result/Ackley/Ackley_30D.txt
elif [ "$1" == "Rastrigin" ] || [ "$1" == "2" ]
then 
    echo "Rastrigin"
    mkdir -p result/Rastrigin/
    ./main.elf --N 1000 --D 2 --k 200 --test_function 2  > result/Rastrigin/Rastrigin_2D.txt
    ./main.elf --N 1000 --D 10 --k 1000 --test_function 2 > result/Rastrigin/Rastrigin_10D.txt
    ./main.elf --N 10000 --D 30 --k 1500 --test_function 2 > result/Rastrigin/Rastrigin_30D.txt
elif [ "$1" == "HappyCat" ] || [ "$1" == "3" ]
then
    echo "HappyCat"
    mkdir -p result/HappyCat/
    ./main.elf --N 1000 --D 2 --k 300 --test_function 3  > result/HappyCat/HappyCat_2D.txt
    ./main.elf --N 1000 --D 10 --k 1500 --test_function 3 > result/HappyCat/HappyCat_10D.txt
    ./main.elf --N 1000 --D 30 --k 3000 --test_function 3 > result/HappyCat/HappyCat_30D.txt
elif [ "$1" == "Rosenbrock" ] || [ "$1" == "4" ]
then
    echo "Rosenbrock"
    mkdir -p result/Rosenbrock/
    ./main.elf --N 1000 --D 2 --k 500 --test_function 4  > result/Rosenbrock/Rosenbrock_2D.txt
    ./main.elf --N 1000 --D 10 --k 1000 --test_function 4 > result/Rosenbrock/Rosenbrock_10D.txt
    ./main.elf --N 1000 --D 30 --k 3000 --test_function 4 > result/Rosenbrock/Rosenbrock_30D.txt
elif [ "$1" == "Zakharov" ] || [ "$1" == "5" ]
then
    echo "Zakharov"
    mkdir -p result/Zakharov/
    ./main.elf --N 1000 --D 2 --k 500 --test_function 5  > result/Zakharov/Zakharov_2D.txt
    ./main.elf --N 1000 --D 10 --k 1000 --test_function 5 > result/Zakharov/Zakharov_10D.txt
    ./main.elf --N 1000 --D 30 --k 5000 --test_function 5 > result/Zakharov/Zakharov_30D.txt
elif [ "$1" == "Michalewicz" ] || [ "$1" == "6" ]
then
    echo "Michalewicz"
    mkdir -p result/Michalewicz/
    ./main.elf --N 1000 --D 2 --k 500 --test_function 6  > result/Michalewicz/Michalewicz_2D.txt
    ./main.elf --N 1000 --D 10 --k 500 --test_function 6 > result/Michalewicz/Michalewicz_10D.txt
    ./main.elf --N 1000 --D 30 --k 2000 --test_function 6 > result/Michalewicz/Michalewicz_30D.txt
elif [ "$1" == "Schwefel" ] || [ "$1" == "7" ]
then
    FN="Schwefel"

    echo ${FN}
    mkdir -p result/${FN}/
    ./main.elf --N 1000 --D 2 --k 100 --test_function 7  > result/${FN}/${FN}_2D.txt
    ./main.elf --N 1000 --D 10 --k 300 --test_function 7 > result/${FN}/${FN}_10D.txt
    ./main.elf --N 1000 --D 30 --k 1000 --test_function 7 > result/${FN}/${FN}_30D.txt
elif [ "$1" == "BentCigar" ] || [ "$1" == "8" ]
then
    FN="BentCigar"

    echo ${FN}
    mkdir -p result/${FN}/
    ./main.elf --N 1000 --D 2 --k 600 --test_function 8  > result/${FN}/${FN}_2D.txt
    ./main.elf --N 1000 --D 10 --k 3000 --test_function 8 > result/${FN}/${FN}_10D.txt
    ./main.elf --N 1000 --D 30 --k 7000 --test_function 8 > result/${FN}/${FN}_30D.txt
elif [ "$1" == "DropWave" ] || [ "$1" == "9" ]
then
    FN="DropWave"

    echo ${FN}
    mkdir -p result/${FN}/
    ./main.elf --N 1000 --D 2 --k 200 --test_function 9  > result/${FN}/${FN}_2D.txt
    ./main.elf --N 1000 --D 10 --k 1000 --test_function 9 > result/${FN}/${FN}_10D.txt
    ./main.elf --N 1000 --D 30 --k 1500 --test_function 9 > result/${FN}/${FN}_30D.txt
elif [ "$1" == "Step" ] || [ "$1" == "10" ]
then
    FN="Step"

    echo ${FN}
    mkdir -p result/${FN}/
    ./main.elf --N 1000 --D 2 --k 200 --test_function 10  > result/${FN}/${FN}_2D.txt
    ./main.elf --N 1000 --D 10 --k 700 --test_function 10 > result/${FN}/${FN}_10D.txt
    ./main.elf --N 1000 --D 30 --k 2000 --test_function 10 > result/${FN}/${FN}_30D.txt
elif [ "$1" == "all" ] || [ "$1" == "-1" ]
then
    echo "Ackley"
    mkdir -p result/Ackley/
    ./main.elf --N 1000 --D 2 --k 500 --test_function 1  > result/Ackley/Ackley_2D.txt
    ./main.elf --N 1000 --D 10 --k 2000 --test_function 1 > result/Ackley/Ackley_10D.txt
    ./main.elf --N 10000 --D 30 --k 3000 --test_function 1 > result/Ackley/Ackley_30D.txt

    echo "Rastrigin"
    mkdir -p result/Rastrigin/
    ./main.elf --N 1000 --D 2 --k 200 --test_function 2  > result/Rastrigin/Rastrigin_2D.txt
    ./main.elf --N 1000 --D 10 --k 1000 --test_function 2 > result/Rastrigin/Rastrigin_10D.txt
    ./main.elf --N 10000 --D 30 --k 1500 --test_function 2 > result/Rastrigin/Rastrigin_30D.txt

    echo "HappyCat"
    mkdir -p result/HappyCat/
    ./main.elf --N 1000 --D 2 --k 300 --test_function 3  > result/HappyCat/HappyCat_2D.txt
    ./main.elf --N 1000 --D 10 --k 1500 --test_function 3 > result/HappyCat/HappyCat_10D.txt
    ./main.elf --N 1000 --D 30 --k 3000 --test_function 3 > result/HappyCat/HappyCat_30D.txt

    echo "Rosenbrock"
    mkdir -p result/Rosenbrock/
    ./main.elf --N 1000 --D 2 --k 500 --test_function 4  > result/Rosenbrock/Rosenbrock_2D.txt
    ./main.elf --N 1000 --D 10 --k 1000 --test_function 4 > result/Rosenbrock/Rosenbrock_10D.txt
    ./main.elf --N 1000 --D 30 --k 3000 --test_function 4 > result/Rosenbrock/Rosenbrock_30D.txt

    echo "Zakharov"
    mkdir -p result/Zakharov/
    ./main.elf --N 1000 --D 2 --k 500 --test_function 5  > result/Zakharov/Zakharov_2D.txt
    ./main.elf --N 1000 --D 10 --k 1000 --test_function 5 > result/Zakharov/Zakharov_10D.txt
    ./main.elf --N 1000 --D 30 --k 5000 --test_function 5 > result/Zakharov/Zakharov_30D.txt

    echo "Michalewicz"
    mkdir -p result/Michalewicz/
    ./main.elf --N 1000 --D 2 --k 500 --test_function 6  > result/Michalewicz/Michalewicz_2D.txt
    ./main.elf --N 1000 --D 10 --k 500 --test_function 6 > result/Michalewicz/Michalewicz_10D.txt
    ./main.elf --N 1000 --D 30 --k 2000 --test_function 6 > result/Michalewicz/Michalewicz_30D.txt

    FN="Schwefel"
    echo ${FN}
    mkdir -p result/${FN}/
    ./main.elf --N 1000 --D 2 --k 100 --test_function 7  > result/${FN}/${FN}_2D.txt
    ./main.elf --N 1000 --D 10 --k 300 --test_function 7 > result/${FN}/${FN}_10D.txt
    ./main.elf --N 1000 --D 30 --k 1000 --test_function 7 > result/${FN}/${FN}_30D.txt

    FN="BentCigar"
    echo ${FN}
    mkdir -p result/${FN}/
    ./main.elf --N 1000 --D 2 --k 600 --test_function 8  > result/${FN}/${FN}_2D.txt
    ./main.elf --N 1000 --D 10 --k 3000 --test_function 8 > result/${FN}/${FN}_10D.txt
    ./main.elf --N 1000 --D 30 --k 7000 --test_function 8 > result/${FN}/${FN}_30D.txt

    FN="DropWave"
    echo ${FN}
    mkdir -p result/${FN}/
    ./main.elf --N 1000 --D 2 --k 200 --test_function 9  > result/${FN}/${FN}_2D.txt
    ./main.elf --N 1000 --D 10 --k 1000 --test_function 9 > result/${FN}/${FN}_10D.txt
    ./main.elf --N 1000 --D 30 --k 1500 --test_function 9 > result/${FN}/${FN}_30D.txt

    FN="Step"
    echo ${FN}
    mkdir -p result/${FN}/
    ./main.elf --N 1000 --D 2 --k 200 --test_function 10  > result/${FN}/${FN}_2D.txt
    ./main.elf --N 1000 --D 10 --k 700 --test_function 10 > result/${FN}/${FN}_10D.txt
    ./main.elf --N 1000 --D 30 --k 2000 --test_function 10 > result/${FN}/${FN}_30D.txt
fi
