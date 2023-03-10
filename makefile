CC=gcc
SRC=$(shell ls ./src)
HDR=$(SRC:.c=.h)
OBJ=$(SRC:.c=.o)
OUT=proximoth
BUILD_DIR=build
BUILD_OBJ_DIR=${BUILD_DIR}/obj
BUILD_BIN_DIR=${BUILD_DIR}/bin
MKDIR_P=mkdir -p
INSTALL_DIR=/usr/local/bin

OBJ_DEP=$(addprefix ${BUILD_OBJ_DIR}/, ${OBJ})
FLAGS=-Wall -pthread  -std=c11 
LIB=-lpcap -lm

proximoth: ${OBJ_DEP}
	$(shell if ! [ -d ${BUILD_BIN_DIR} ]; then ${MKDIR_P} ${BUILD_BIN_DIR}; fi)
	@${CC} ${FLAGS} ${OBJ_DEP} ${LIB} -o ${BUILD_BIN_DIR}/${OUT}

${BUILD_OBJ_DIR}/%.o: src/%.c include/%.h
	$(shell if ! [ -d ${BUILD_OBJ_DIR} ]; then ${MKDIR_P} ${BUILD_OBJ_DIR}; fi)
	${CC} -c $< -o $@

run:
	@cd ${BUILD_DIR}; ./${OUT}


clean:
	@rm -rf ${BUILD_DIR}


install: 
	@cp ${BUILD_BIN_DIR}/${OUT} ${INSTALL_DIR}

uninstall:
	@rm -f ${INSTALL_DIR}/${OUT}