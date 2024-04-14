CC:=g++
FLAG:=-std=c++23 -Wall -Wextra -O3
SOURCE:=main.cpp
OBJ:=main.o
ELF:=main.elf
HEADER_DIR:=src
HEADER:=$(HEADER_DIR)/vector.h $(HEADER_DIR)/test_function.h

dep: $(SOURCE) $(HEADER_DIR) $(HEADER)
	$(CC) $(FLAG) -I $(HEADER_DIR) -c $(SOURCE)
all: $(OBJ)
	$(CC) $(OBJ) -o $(ELF)

.PHONY: clean
clean:
	-rm -rf *.o

