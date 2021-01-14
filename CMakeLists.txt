
CMAKE_MINIMUM_REQUIRED (VERSION 3.4)
PROJECT (AlouFEM)

FUNCTION(COLLECT_SRC_FROM_DIRS DIRS)
  UNSET(SOURCE_FILES PARENT_SCOPE)
  FOREACH(dir ${${DIRS}})
    FILE(GLOB source_file_in_dir ${dir}/[^.]*.cpp ${dir}/[^.]*.hxx)
	LIST(APPEND SOURCE_FILE_LIST ${source_file_in_dir})
  ENDFOREACH()
  SET(SOURCE_FILES ${SOURCE_FILE_LIST} PARENT_SCOPE)
ENDFUNCTION ()



#SET(ENV{BOOST_ROOT} D:/Boost/boost_1_75_0_binary)
#if (NOT "$ENV{BOOST_ROOT}" STREQUAL "")
if (DEFINED ENV{BOOST_ROOT})
  #MESSAGE("ENV{BOOST_ROOT} defined == $ENV{BOOST_ROOT}")
  #set(BOOST_RO "$ENV{EnvironmentVariableName}" CACHE INTERNAL "Copied from environment variable")
ENDIF()

#Command to compile boost library
#b2.exe --prefix=D:\Boost\boost_1_75_0_binary toolset=msvc-14.0 variant=debug link=static threading=multi runtime-link=shared install
FIND_PACKAGE(Boost REQUIRED COMPONENTS filesystem exception program_options log)

SET(Boost_USE_DEBUG_LIBS ON)
SET(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTITHREADED      ON)
#set(Boost_USE_STATIC_RUNTIME ON)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
endif()

ENABLE_TESTING()

ADD_SUBDIRECTORY(vector_validation)
ADD_SUBDIRECTORY(AlouFEM)
ADD_SUBDIRECTORY(boost_test)



