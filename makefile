CC=g++

SRC=src/proximoth.cpp  src/proximoth_radiotap.cpp src/proximoth_display_manager.cpp \
src/proximoth_config.cpp src/proximoth_frame_sniffer.cpp src/proximoth_error.cpp src/proximoth_misc_tools.cpp \
src/proximoth_rts_injector.cpp src/proximoth_threads.cpp src/proximoth_signal.cpp \
src/proximoth_interface_mgmt.cpp


HDR=include/proximoth.hpp  include/proximoth_radiotap.hpp include/proximoth_display_manager.hpp \
include/proximoth_config.hpp include/proximoth_frame_sniffer.hpp include/proximoth_error.hpp include/proximoth_misc_tools.hpp \
include/proximoth_rts_injector.hpp include/proximoth_threads.hpp include/proximoth_signal.hpp \
include/proximoth_interface_mgmt.hpp


FLAGS=-pthread
LIB=-lpcap
OUT=./proximoth

proximoth: ${SRC} ${HDR}
	@echo Compiling Proximoth...
	@${CC} ${SRC} ${LIB} ${FLAGS} -o ${OUT}

run:
	@clear; ./${OUT}

compile-run:
	@make proximoth
	@clear
	@./${OUT}