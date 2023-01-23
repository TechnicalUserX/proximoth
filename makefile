CC=gcc
SRC=$(shell ls ./src)
HDR=$(SRC:.c=.h)
OBJ=$(SRC:.c=.o)
OUT=proximoth
OBJ_DEP=$(addprefix  ./obj/, ${OBJ})
BUILD_DIR=./build
FLAGS=-Wall -pthread  -std=c11 
LIB=-lpcap -lm

proximoth: ${OBJ_DEP}
	@${CC} ${FLAGS} ${OBJ_DEP} ${LIB} -o ./${BUILD_DIR}/${OUT}

obj/%.o: src/%.c include/%.h
	${CC} -c $< -o $@

run:
	@cd ${BUILD_DIR}; ./${OUT}


clean:
	@rm ./obj/*
	@rm ./build/${OUT}

install: 
	@cp ./build/proximoth /usr/local/bin

	