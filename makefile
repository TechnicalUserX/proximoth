# Proximoth Makefile
# Author: TechnicalUserX

# Project
PROJECT=proximoth

# Versioning
VERSION_MAJOR=1
VERSION_MINOR=0
VERSION_PATCH=0
VERSION_RELEASE=beta.13

ifneq ("${VERSION_RELEASE}","")
VERSION=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_RELEASE}
else
VERSION=${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
endif

# Miscellaneous
MKDIR_P=mkdir -p

# Source files, headers, objects
SRC_EXT=c
HDR_EXT=h
OBJ_EXT=o
SRC_DIR=src
HDR_DIR=include
SRC = $(shell find $(SRC_DIR) -type f -name "*.${SRC_EXT}")
OBJ = $(patsubst $(SRC_DIR)/%, $(BUILD_OBJ_DIR)/%, $(SRC:.${SRC_EXT}=.${OBJ_EXT}))
MAN_DIR=manpages
IMG_DIR=images
TEST_DIR=tests

# Building
BUILD_DIR=build
BUILD_OBJ_DIR=${BUILD_DIR}/obj
BUILD_BIN_DIR=${BUILD_DIR}/bin
BUILD_HDR_DIR=${BUILD_DIR}/include
BUILD_MAN_DIR=${BUILD_DIR}/manpages

# Manual Pages
MAN_SEC=1
MAN_PAGE=${BUILD_MAN_DIR}/${PROJECT}.${MAN_SEC}

# Installing
INSTALL_BIN_DIR=/usr/local/bin
INSTALL_MAN_DIR=/usr/local/share/man/man1
INSTALL_DOC_DIR=/usr/local/share/doc/${PROJECT}

# Compilation
CC=gcc
CFLAGS=-Wall -Werror -Wpedantic -std=c11 -I${HDR_DIR} -I${BUILD_HDR_DIR}
LDFLAGS=-lpcap -lm -pthread

# Version Header
VERSION_HDR=${BUILD_HDR_DIR}/${PROJECT}/version/version.${HDR_EXT}


# Recipies
${PROJECT}: ${VERSION_HDR} ${OBJ} ${MAN_PAGE}
	@$(shell if ! [ -d ${BUILD_BIN_DIR} ]; then ${MKDIR_P} ${BUILD_BIN_DIR}; fi)
	@${CC} ${FLAGS} ${OBJ} ${LDFLAGS} -o ${BUILD_BIN_DIR}/${PROJECT}

${BUILD_OBJ_DIR}/%.${OBJ_EXT}: ${SRC_DIR}/%.${SRC_EXT}
	@$(shell if ! [ -d $(dir $@) ]; then ${MKDIR_P} $(dir $@); fi)
	@${CC} ${CFLAGS} -c $< -o $@

${VERSION_HDR}:
	@$(shell if ! [ -d ${BUILD_HDR_DIR}/${PROJECT}/version ]; then ${MKDIR_P} ${BUILD_HDR_DIR}/${PROJECT}/version; fi)
	@sed 	-e 's/@PROXIMOTH_VERSION_CONFIG@/${VERSION}/g' \
			-e 's/@PROXIMOTH_VERSION_MAJOR_CONFIG@/${VERSION_MAJOR}/g' \
			-e 's/@PROXIMOTH_VERSION_MINOR_CONFIG@/${VERSION_MINOR}/g' \
			-e 's/@PROXIMOTH_VERSION_PATCH_CONFIG@/${VERSION_PATCH}/g' \
			-e 's/@PROXIMOTH_VERSION_RELEASE_CONFIG@/${VERSION_RELEASE}/g' \
			${HDR_DIR}/version.${HDR_EXT}.in > $@


${MAN_PAGE}:
ifneq (${PROXIMOTH_DOCKER},true)
	$(shell if ! [ -d ${BUILD_MAN_DIR} ]; then ${MKDIR_P} ${BUILD_MAN_DIR}; fi)
	@sed 	-e 's/@PROXIMOTH_VERSION@/${VERSION}/g' \
	 		-e 's#@PROXIMOTH_DATE@#${DATE}#g' \
	  		${MAN_DIR}/${PROJECT}.${MAN_SEC}.in > $@
	@gzip -k ${BUILD_MAN_DIR}/${PROJECT}.${MAN_SEC}
endif

build: ${PROJECT}

clean:
	@rm -rf ${BUILD_DIR}

install: 
	@install -D ${BUILD_BIN_DIR}/${PROJECT} --target-directory ${INSTALL_BIN_DIR}
ifneq (${PROXIMOTH_DOCKER},true)
	@install -D ${BUILD_MAN_DIR}/${PROJECT}.${MAN_SEC}.gz --target-directory ${INSTALL_MAN_DIR}
	@install -D ${IMG_DIR}/* --target-directory ${INSTALL_DOC_DIR}/${IMG_DIR}
	@install -D LICENSE --target-directory ${INSTALL_DOC_DIR}
endif

uninstall:
	@rm -f ${INSTALL_BIN_DIR}/${PROJECT}
ifneq (${PROXIMOTH_DOCKER},true)
	@rm -f ${INSTALL_MAN_DIR}/${PROJECT}.${MAN_SEC}.gz
	@rm -rf ${INSTALL_DOC_DIR}
endif
