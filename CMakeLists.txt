# Dice Strategy Simulator - Jed Fakhfekh - https://github.com/ougi-washi

## General
cmake_minimum_required(VERSION 3.22.1)

project(DiceStrategySimulator LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

# Directory setup
set(INC_DIR "include")
set(SRC_DIR "src")
set(BIN_DIR "bin")

# setup library macro
macro(setup_library arg_lib_name arg_source_path arg_include_path)
	message(STATUS "Adding ${arg_lib_name}")
	file(GLOB CURRENT_SRC_FILES	"${arg_source_path}/*.c" 
								"${arg_source_path}/*.cpp"
								"${arg_source_path}/**/*.c" 
								"${arg_source_path}/**/*.cpp")
	add_library(${arg_lib_name} STATIC	${CURRENT_SRC_FILES})
	target_include_directories(${arg_lib_name} PUBLIC ${arg_include_path})
endmacro()

# setup executable macro
macro(setup_executable arg_exec_name modules)
	message(STATUS "Generating executable ${arg_exec_name}")
    add_executable(${arg_exec_name} "${arg_exec_name}.cpp")
    target_link_libraries(${arg_exec_name} PUBLIC main_module)
    set_target_properties(${arg_exec_name} PROPERTIES
                          RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${BIN_DIR})
    
endmacro()

find_package(Threads REQUIRED)

# main module
set(MAIN_MODULE_INCLUDES ${PROJECT_SOURCE_DIR}/${INC_DIR} ${PROJECT_SOURCE_DIR}/${LIB_DIR})
setup_library(main_module ${SRC_DIR} "${MAIN_MODULE_INCLUDES}" Threads::Threads)

# Executables
setup_executable(martingale main_module)
