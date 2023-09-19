# Proximoth Test Makefile
# Author: TechnicalUserX

# Compilation
CXX=g++
CXXFLAGS=-Wall -std=c++11 -I../include
LIB=-lgtest -lgtest_main

# Building
BUILD_DIR=../build
BUILD_TEST_DIR=${BUILD_DIR}/tests

# Miscellaneous
MKDIR_P=mkdir -p

# Source files
SRC=../src
SRC_TEST_001=${SRC}/wireless/mac.c
SRC_TEST_002=${SRC}/utility/error.c

# Test information
TEST_EXT=cpp
TESTS=	${BUILD_TEST_DIR}/test-proximoth-001 \
		${BUILD_TEST_DIR}/test-proximoth-002


${TESTS}:

test: ${TESTS}
	@for t in ${TESTS}; do $$t; done

# Test Recipes
${BUILD_TEST_DIR}/test-proximoth-001:
	@${MKDIR_P} ${BUILD_TEST_DIR}
	@${CXX} $(notdir $@).${TEST_EXT} ${SRC_TEST_001} ${CXXFLAGS} ${LIB} -o $@

${BUILD_TEST_DIR}/test-proximoth-002:
	@${MKDIR_P} ${BUILD_TEST_DIR}
	@${CXX} $(notdir $@).${TEST_EXT} ${SRC_TEST_002} ${CXXFLAGS} ${LIB} -o $@


clean:
	@rm -rf ${BUILD_TEST_DIR}
