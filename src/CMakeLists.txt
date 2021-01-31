project(abapscript)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

cmake_minimum_required(VERSION 3.13)

###############################################################################
include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)
###############################################################################


# place all compiled results into the top level dir
# should be a solution to the problem that exectuables 
# cannot find their dlls
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


find_package(TCL REQUIRED)

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )


#find_package(SAPNWRFC REQUIRED)
if (WIN32)
find_path(SAPNWRFC_INCLUDE_DIR
  NAMES sapnwrfc.h
  PATHS ${CMAKE_CURRENT_SOURCE_DIR}/nwrfcsdk.win/include
)
find_library(SAPNWRFC_LIBRARY
  NAMES sapnwrfc libsapucum
  PATHS ${CMAKE_CURRENT_SOURCE_DIR}/nwrfcsdk.win/lib
)
include_directories(${TCL_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/nwrfcsdk.win/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/nwrfcsdk.win/lib)
add_definitions(-DSAPwithUNICODE -DUNICODE -D_UNICODE -DSAPwithTHREADS)

endif (WIN32)

if (UNIX)
find_path(SAPNWRFC_INCLUDE_DIR
  NAMES sapnwrfc.h
  PATHS ${CMAKE_CURRENT_SOURCE_DIR}/nwrfcsdk.linux/include
)
find_library(SAPNWRFC_LIBRARY
  NAMES sapnwrfc libsapucum
  PATHS ${CMAKE_CURRENT_SOURCE_DIR}/nwrfcsdk.linux/lib
)
include_directories(${TCL_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/nwrfcsdk.linux/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/nwrfcsdk.linux/lib)
add_definitions( -DSAPonUNIX -DSAPonLINUX )
#add_definitions( -DSAPwithUNICODE -DUNICODE -D_UNICODE -DSAPwithTHREADS)
add_definitions(  -DSAPonLIN -DSAPwithUNICODE -DSAPwithTHREADS)
endif (UNIX)
# Finally the library itself




include_directories(${SAPNWRFC_INCLUDE_DIRS})
set(LIBS ${LIBS} ${SAPNWRFC_LIBRARIES})

#add_subdirectory("src")
add_executable(t1 t1.c)
target_link_libraries(t1 yaml)
target_link_libraries(t1 cnt mem)
install(TARGETS t1 RUNTIME DESTINATION bin)
#add_subdirectory("test")


###############################################################################
# Unit tests
#enable_testing()
#add_test(NAME test_main COMMAND test_main)
